#include "FakeSynthTimer.h"

static bool initialized = false;
static void (*timerCallback)(void);

static bool interruptEnabled = false;
static unsigned int disables = 0;
static unsigned int enables = 0;

static void dummyFunc (void) {};

void SynthTimer_Open(void (*callBack)(void))
{
    initialized = true;
    interruptEnabled = true;
    timerCallback = callBack;

    enables = 0;
    disables = 0;
}


void SynthTimer_Close(void)
{
    initialized = false;
    interruptEnabled = false;
    timerCallback = dummyFunc;
}

void SynthTimer_DisableInterrupt(void)
{
    interruptEnabled = false;
    disables += 1;
}

void SynthTimer_EnableInterrupt(void)
{
    interruptEnabled = true;
    enables += 1;
}

bool FakeSynthTimer_Initialized(void)
{
    return initialized;
}

void FakeSynthTimer_Interrupt(void)
{
    timerCallback();
}

unsigned int FakeSynthTimer_GetEnables(void)
{
    return enables;
}

unsigned int FakeSynthTimer_GetDisables(void)
{
    return disables;
}