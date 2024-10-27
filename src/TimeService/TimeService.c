#include <stdlib.h>

#include "TimeService.h"
#include "SystemTimer.h"

typedef struct SoftTimerStruct
{
    timeMicroseconds startTime;
    timeMicroseconds timerPeriod;
} SoftTimerStruct;

// Esta variável guarda o tempo de sistema
// Como é atualizada por meio de interrupção, deve ser do tipo volátil (volatile)
static volatile timeMicroseconds systemTime = 0;

static void timerInterruptCallback(timeMicroseconds useconds)
{
    systemTime += useconds;
}

void TimeService_Create(void)
{
    systemTime = 0;
    SystemTimer_Create(timerInterruptCallback);
}

void TimeService_Destroy(void)
{
    SystemTimer_Destroy();
}

static void readCurrentTime(timeMicroseconds * time)
{
    SystemTimer_Pause();
    *time = systemTime;
    SystemTimer_Continue();
}

timeMicroseconds TimeService_Time(void)
{
    timeMicroseconds time = 0;
    readCurrentTime(&time);
    return time;
}

SoftTimer TimeService_CreateTimer(timeMicroseconds timerPeriod)
{
    SoftTimer timer = (SoftTimerStruct*) malloc(sizeof(SoftTimerStruct));
    
    readCurrentTime(&(timer->startTime));
    timer->timerPeriod = timerPeriod;

    return timer;
}

bool TimeService_DestroyTimer(SoftTimer timer)
{
    if(!timer) return false;

    free(timer);
    return true;
} 

bool TimeService_CheckTimer(SoftTimer timer)
{
    if (!timer) return false;

    timeMicroseconds currentTime;
    readCurrentTime(&currentTime);

    /* Esta equação funciona mesmo para casos de overflow, mas depende das variáveis serem 'unsigned' */
    if ( timer->timerPeriod < (currentTime - timer->startTime))
    {
        timer->startTime = currentTime;
        return true;
    }

    return false;
}

void TimeService_ResetTimer(SoftTimer timer)
{
    if (!timer) return;
     
    readCurrentTime( &(timer->startTime));
}