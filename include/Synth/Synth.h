#ifndef D_SYNTH_H
#define D_SYNTH_H

#include <stdbool.h>
#include <stdint.h>

#define NOTE_SAMPLES_MAX_SIZE 200

typedef enum
{ squareSignal, sampleSignal}
KeyType;

typedef enum
{ Note_C4, Note_D4, Note_E4, Note_F4, Note_G4, Note_A4, Note_B4, Note_C5, KEYS_SIZE}
Note;

typedef struct 
{
    bool pressed;
    unsigned int period;
    unsigned int counter;
    signed char  amplitude;
}SquareWaveKey;

typedef struct
{
    bool pressed;
    unsigned int periodSize;
    unsigned int periodCounter;
    int32_t samples[NOTE_SAMPLES_MAX_SIZE];
    int32_t amplitude;
    unsigned long tickCounter;
    int32_t ADSRGain;
}SampleWaveKey;

void Synth_Open(void);
void Synth_Close(void);

void Synth_SetKeys(KeyType type);

void Synth_Press(Note note);
void Synth_Release(Note note);

void Synth_Run(void);


/* TestFuncs */
char            Synth_GetNextDACValue(void);
SampleWaveKey   Synth_GetNoteSampleWaveKey(Note note);

#endif /*D_SYNTH_H*/