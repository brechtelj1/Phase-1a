#include "phase1Int.h"
#include "usloss.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

extern  USLOSS_PTE  *P3_AllocatePageTable(int cid);
extern  void        P3_FreePageTable(int cid);

typedef struct Context {
    void            (*startFunc)(void *);
    void            *startArg;
    USLOSS_Context  context;
    void            *stackStart;
    int             available;
    int             id;
    // you'll need more stuff here
} Context;

static Context contexts[P1_MAXPROC];
static int currentCid = -1;

void checkMode();

/*
 * Helper function to call func passed to P1ContextCreate with its arg.
 */
static void launch(void){
    assert(contexts[currentCid].startFunc != NULL);
    contexts[currentCid].startFunc(contexts[currentCid].startArg);
}


void P1ContextInit(void){
    // initialize contexts
    checkMode();
    int i;
    for(i = 0; i < P1_MAXPROC; i++){
        contexts[i].available = 1;
        contexts[i].id = i;
    }
}


int P1ContextCreate(void (*func)(void *), void *arg, int stacksize, int *cid) {
    int result = P1_SUCCESS;
    int i = 0;
    USLOSS_PTE *pageTable;
    printf("creating context %d\n",*cid);
    checkMode();
    // if stacksize smaller than min stack ERROR
    if(stacksize < USLOSS_MIN_STACK){
        printf("invalid stack\n");
        return P1_INVALID_STACK;
    }
    // find a free context and initialize it
    while(contexts[i].available == 0){
        // check to prevent context overflow
        if(i >= P1_MAXPROC){
            printf("too many \n");
            return P1_TOO_MANY_CONTEXTS;
        }
        i++;
    }
    *cid = i;

    // allocate the stack, specify the startFunc, etc.
    contexts[i].startFunc = func;
    contexts[i].available = 0;
    contexts[i].startArg = arg;
    contexts[i].stackStart = malloc(stacksize);
    
    // initialize the context and allocate page table within call and launch context
    pageTable = P3_AllocatePageTable(*cid);
    USLOSS_ContextInit(&contexts[i].context,contexts[i].stackStart,stacksize,pageTable,launch);
    printf("create result = %d\n",result);
    return result;
}

// switch to the specified context
int P1ContextSwitch(int cid) {
    int result = P1_SUCCESS;
    checkMode();
    // init if this is the first context
    if(currentCid == -1){
        currentCid = cid;
        USLOSS_ContextSwitch(NULL, &contexts[cid].context);
        return result;
    }
    // checks if the switch to itself to avoid overhead
    if(cid == currentCid){
        return result;
    }

    // checks to see if id trying to switch to is created
    if(contexts[cid].available == 1){
        return P1_INVALID_CID;
    }
    // switch to desired context and sets new current context id
    USLOSS_ContextSwitch(&contexts[currentCid].context, &contexts[cid].context);
    currentCid = cid;
    printf("switch result = %d\n",result);
    return result;
}

// Frees the stack and marks the context as unused
int P1ContextFree(int cid) {
    int result = P1_SUCCESS;
    checkMode();
    if(contexts[cid].available == 1){
        return P1_INVALID_CID;
    }
    else if(cid == currentCid){
        return P1_CONTEXT_IN_USE;
    }
    else{
        P3_FreePageTable(cid);
        contexts[cid].available = 1;
        free(contexts[cid].stackStart);
        contexts[cid].startArg = NULL;
        contexts[cid].startFunc = NULL;
    }
    return result;
}

// enable the interrupts by setting PSR bit to 1
void P1EnableInterrupts(void) {
    // set the interrupt bit in the PSR
    unsigned int psr = USLOSS_PsrSet(1);
    checkMode();
    //printf("enabled = %d\n",psr);
    if(USLOSS_DEV_OK == psr){
        USLOSS_Halt(0);
    }
    USLOSS_Halt(0);
}

/*
 * Returns true if interrupts were enabled, false otherwise.
 */
int P1DisableInterrupts(void) {
    int enabled;
    // get the PSR and check if it is activated or not. 
    int psr = USLOSS_PsrGet();
    // set enabled to TRUE if interrupts are already enabled
    checkMode();
    if(psr == 1){
        enabled = TRUE;

    }
    else{
        enabled = FALSE;
    }
    // clear the interrupt bit in the PSR
    psr = USLOSS_PsrSet(0);
    printf("disabled = %d\n",enabled);
    return enabled;
}

void checkMode(){
    if(USLOSS_PSR_CURRENT_MODE == 0){
        printf("ERROR: Illegal Instruction\n");
        USLOSS_Halt(0);
    }    
}
