#include "FakeSynthTimer.h"

static bool initialized = false;
static void (*timerCallback)(void);


static void dummyFunc (void) {};

void SynthTimer_Open(void (*callBack)(void))
{
    initialized = true;
    timerCallback = callBack;
}


void SynthTimer_Close(void)
{
    initialized = false;
    timerCallback = dummyFunc;
}

bool FakeSynthTimer_Initialized(void)
{
    return initialized;
}