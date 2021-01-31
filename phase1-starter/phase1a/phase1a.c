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

static Context   contexts[P1_MAXPROC];

static int currentCid = -1;

/*
 * Helper function to call func passed to P1ContextCreate with its arg.
 */
static void launch(void)
{
    assert(contexts[currentCid].startFunc != NULL);
    contexts[currentCid].startFunc(contexts[currentCid].startArg);
}

void P1ContextInit(void)
{
    // initialize contexts
    int i;
    for(i = 0; i < P1_MAXPROC; i++){
        contexts[i].available = 1;
        contexts[i].id = i;
    }
}

int P1ContextCreate(void (*func)(void *), void *arg, int stacksize, int *cid) {
    int result = P1_SUCCESS;
    int i = 0;
    // find a free context and initialize it
    // allocate the stack, specify the startFunc, etc.
    if(stacksize < USLOSS_MIN_STACK){
        return P1_INVALID_STACK;
    }
    while(contexts[i].available == 0){
        if(i >= P1_MAXPROC){
            return P1_TOO_MANY_CONTEXTS;
        }
        i++;
    }
    *cid = i;
    contexts[i].startFunc = func;
    contexts[i].available = 0;
    contexts[i].startArg = arg;
    contexts[i].stackStart = malloc(stacksize);
    currentCid = i;
    P3_AllocatePageTable();
    launch();
    return result;
}

int P1ContextSwitch(int cid) {
    int result = P1_SUCCESS;
    // switch to the specified context
    // checks if the switch to itself
    if(cid == currentCid){
        return result;
    }
    // checks to see if id trying to switch to is created
    if(contexts[cid].available == 1){
        return P1_INVALID_CID;
    }
    // switch to desired context and sets new current context id
    USLOSS_ContextSwitch(currentCid, cid);
    currentCid = cid;
    return result;
}

int P1ContextFree(int cid) {
    int result = P1_SUCCESS;
    // free the stack and mark the context as unused
    return result;
}


void 
P1EnableInterrupts(void) 
{
    // set the interrupt bit in the PSR
    USLOSS_PSR_CURRENT_INT = 1;
}

/*
 * Returns true if interrupts were enabled, false otherwise.
 */
int 
P1DisableInterrupts(void) 
{
    int enabled = FALSE;
    // set enabled to TRUE if interrupts are already enabled
    // clear the interrupt bit in the PSR
    return enabled;
}
