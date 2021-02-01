#include <phase1.h>
#include <phase1Int.h>
#include <assert.h>
#include "tester.h"

void
dummy(int device, void *arg)
{}

static void
Output(void *arg) 
{
    char *msg = (char *) arg;

    USLOSS_Console("%s", msg);
    PASSED();
    USLOSS_Halt(0);
}

void
startup(int argc, char **argv)
{
    int rc;
	USLOSS_IntVec[USLOSS_ILLEGAL_INT] = dummy;
    P1ContextInit();
	int currPsr;
	currPsr = (USLOSS_PsrGet() >> 1) & 0x1;
	rc = P1DisableInterrupts();
	USLOSS_Console("RC: %d -- Expected: %d\n", rc, FALSE);
    // should not return
    TEST(rc, FALSE);
    // if we get here we failed the test.
	USLOSS_Halt(0);
}

void test_setup(int argc, char **argv) {}

void test_cleanup(int argc, char **argv) {}

void finish(int argc, char **argv) {}
