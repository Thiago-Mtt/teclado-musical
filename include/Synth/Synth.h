#ifndef D_SYNTH_H
#define D_SYNTH_H

typedef enum
{ C4, D4, E4, F4, G4, A4, B4, C5, KEYS_SIZE}
Note;

void Synth_Open(void);
void Synth_Close(void);

void Synth_Press(Note note);
void Synth_Release(Note note);

void Synth_Run(void);


#endif /*D_SYNTH_H*/