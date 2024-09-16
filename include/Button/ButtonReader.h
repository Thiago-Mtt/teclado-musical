#ifndef D_BUTTONREADER_H
#define D_BUTTONREADER_H

void ButtonReader_Open(void);
void ButtonReader_Close(void);

unsigned int ButtonReader_Read(unsigned int button);

#endif /*D_BUTTONREADER_H*/