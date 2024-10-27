#ifndef D_FAKETIMER_H
#define D_FAKETIMER_H

#include "SystemTimer.h"

void FakeSystemTimer_AddTime(timeMicroseconds useconds);
int  FakeSystemTimer_GetPauses(void);
int  FakeSystemTimer_GetContinues(void);

#endif /*D_FAKETIMER_H*/