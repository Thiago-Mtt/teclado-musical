#ifndef D_SYSTEMCLOCK_H
#define D_SYSTEMCLOCK_H

#include <stdbool.h>
#include "TimeTypes.h"

void            TimeService_Create      (void);
void            TimeService_Destroy     (void);

/*Retorna o tempo em microsegundos*/
timeMicroseconds    TimeService_Time        (void);

typedef struct SoftTimerStruct * SoftTimer;

SoftTimer   TimeService_CreateTimer     (timeMicroseconds timerPeriod);
bool        TimeService_DestroyTimer    (SoftTimer softTimer);
bool        TimeService_CheckTimer      (SoftTimer softTimer);
void        TimeService_ResetTimer      (SoftTimer softTimer);

#endif /*D_SYSTEMCLOCK_H*/