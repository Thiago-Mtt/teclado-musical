#ifndef D_SYNTHTIMER_H
#define D_SYNTHTIMER_H

void SynthTimer_Open(void (*callBack)(void));
void SynthTimer_Close(void);

void SynthTimer_DisableInterrupt(void);
void SynthTimer_EnableInterrupt(void);


#endif /*D_SYNTHTIMER_H*/