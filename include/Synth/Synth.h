#ifndef D_SYNTH_H
#define D_SYNTH_H

typedef enum
{ C3, D3, E3, F3, G3, A3, B3, C4}
Note;

void Synth_Open(void);
void Synth_Close(void);

void Synth_Press(Note note);
void Synth_Release(Note note);

void Synth_Run(void);


#endif /*D_SYNTH_H*/