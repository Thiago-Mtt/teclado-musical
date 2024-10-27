#include "FakeSystemTimer.h"

static void emptyFunc(timeMicroseconds useconds)
{   
    /* Função inútil para ocupar 'timercallback' e evitar erro no programa caso seja chamada */
    timeMicroseconds trashVar = 0;
    trashVar += useconds;
}

static Callback timerCallback = emptyFunc;
static int pauses = 0;
static int continues = 0;

void SystemTimer_Create(Callback systemClockCallback)
{
    timerCallback = systemClockCallback;
    pauses = 0;
    continues = 0;
}

void SystemTimer_Destroy(void)
{
    timerCallback = emptyFunc;
    pauses = 0;
    continues = 0;
}

void SystemTimer_Pause(void)
{
    pauses += 1;
}

void SystemTimer_Continue(void)
{
    continues += 1;
}

void FakeSystemTimer_AddTime(timeMicroseconds useconds)
{
    timerCallback(useconds);
}

int  FakeSystemTimer_GetPauses(void)
{
    return pauses;
}

int  FakeSystemTimer_GetContinues(void)
{
    return continues;
}