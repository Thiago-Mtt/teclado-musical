#ifndef D_BUTTONREADER_H
#define D_BUTTONREADER_H

#include "ButtonState.h"

void ButtonReader_Open(void);
void ButtonReader_Close(void);

ButtonState ButtonReader_Read(unsigned int button);

#endif /*D_BUTTONREADER_H*/