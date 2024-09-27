#ifndef D_SYNTH_H
#define D_SYNTH_H

typedef enum
{ Note_C4, Note_D4, Note_E4, Note_F4, Note_G4, Note_A4, Note_B4, Note_C5, KEYS_SIZE}
Note;

void Synth_Open(void);
void Synth_Close(void);

void Synth_Press(Note note);
void Synth_Release(Note note);

void Synth_Run(void);


#endif /*D_SYNTH_H*/