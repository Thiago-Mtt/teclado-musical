#ifndef D_FAKESYNTHDAC_H
#define D_FAKESYNTHDAC_H

#include "SynthDAC.h"
#include <stdbool.h>

bool FakeSynthDAC_Initialized   (void);
bool FakeSynthDAC_GetLastWrite  (unsigned char * writeValue);
void FakeSynthDAC_ClearLastWrite(void);

#endif /*D_FAKESYNTHDAC_H*/