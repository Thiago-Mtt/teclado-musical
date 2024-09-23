#ifndef D_FAKESYNTHTIMER_H
#define D_FAKESYNTHTIMER_H

#include "SynthTimer.h"
#include <stdbool.h>

bool FakeSynthTimer_Initialized(void);
void FakeSynthTimer_Interrupt  (void);

#endif /*D_FAKESYNTHTIMER_H*/