
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <usloss.h>
#include <phase1Int.h>

#define CHECKKERNEL() \
    if ((USLOSS_PsrGet() & USLOSS_PSR_CURRENT_MODE) == 0) USLOSS_IllegalInstruction()

typedef struct Lock
{
    char        name[P1_MAXNAME];
    int         locked;
    int         inuse;
    // more fields here
} Lock;

static Lock locks[P1_MAXLOCKS];

void 
P1LockInit(void) 
{
    static int initialized = FALSE;
    CHECKKERNEL();
    if (!initialized) {
        P1ProcInit();
        for (int i = 0; i < P1_MAXLOCKS; i++) {
            locks[i].inuse = FALSE;
            // initialize rest of lock here
        }
    }
    initialized = TRUE;
}

int P1_LockCreate(char *name, int *lid)
{
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // disable interrupts
    // check parameters
    // find a free Lock and initialize it
    // re-enable interrupts if they were previously enabled
    return result;
}

int P1_LockFree(int lid) 
{
    int     result = P1_SUCCESS;
    CHECKKERNEL();
    // more code here
    return result;
}

int P1_Lock(int lid) 
{
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // disable interrupts
    // check if current process already holds lock
    // while lock is already held
    //      set state to P1_STATE_BLOCKED
    //      P1Dispatch(FALSE);
    // record that lock is now held by this process
    // re-enable interrupts if they were previously enabled
    return result;
}

int P1_Unlock(int lid) 
{
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // disable interrupts
    // check if current process holds lock
    // mark lock as unlocked
    // if a process is waiting for this lock
    //      set the process's state to P1_STATE_READY
    //      P1Dispatch(FALSE);
    // re-enable interrupts if they were previously enabled
    return result;
}

int P1_LockName(int lid, char *name, int len) {
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // more code here
    return result;
}

/*
 * Condition variable functions.
 */

void P1CondInit(void) {
    CHECKKERNEL();
    // more code here
}

int P1_CondCreate(char *name, int lid, int *vid) {
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // more code here
    return result;
}

int P1_CondFree(int vid) {
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // more code here
    return result;
}

int P1_Wait(int vid) {
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // more code here
    return result;
}

int P1_Signal(int vid) {
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // more code here
    return result;
}

int P1_Broadcast(int vid) {
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // more code here
    return result;
}

int P1_NakedSignal(int vid) {
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // more code here
    return result;
}

int P1_CondName(int vid, char *name, int len) {
    int result = P1_SUCCESS;
    CHECKKERNEL();
    // more code here
    return result;
}
