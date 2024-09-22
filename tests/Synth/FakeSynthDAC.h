#ifndef D_FAKESYNTHDAC_H
#define D_FAKESYNTHDAC_H

#include "SynthDAC.h"
#include <stdbool.h>

bool FakeSynthDAC_Initialized   (void);
bool FakeSynthDAC_GetLastWrite  (unsigned char * writeValue);

#endif /*D_FAKESYNTHDAC_H*/