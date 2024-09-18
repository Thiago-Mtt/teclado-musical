#ifndef D_FAKEBUTTONREADER_H
#define D_FAKEBUTTONREADER_H

#include "ButtonReader.h"

int FakeButtonReader_IsInitialized();

void FakeButtonReader_PressButton(unsigned int button);
void FakeButtonReader_ReleaseButton(unsigned int button);

#endif /*D_FAKEBUTTONREADER_H*/