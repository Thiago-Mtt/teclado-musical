#ifndef D_FAKESYNTHTIMER_H
#define D_FAKESYNTHTIMER_H

#include "SynthTimer.h"
#include <stdbool.h>

bool FakeSynthTimer_Initialized(void);
void FakeSynthTimer_Interrupt  (void);

unsigned int FakeSynthTimer_GetDisables(void);
unsigned int FakeSynthTimer_GetEnables(void);


#endif /*D_FAKESYNTHTIMER_H*/