#include "Synth.h"
#include "SynthDAC.h"
#include "SynthTimer.h"

#include <stdbool.h>

typedef struct 
{
    bool pressed;
    unsigned int counterMax;
    unsigned int counter;
}SquareWaveKey;


const unsigned char dacMidLevel = 128;

static SquareWaveKey squareKeys[KEYS_SIZE];

static volatile signed char signalSum = 0;
static volatile bool processNextDAC = false;

static void updateDAC (void)
{
    SynthDAC_Write(dacMidLevel + signalSum);
    processNextDAC = true;
}

static void initKeys(void)
{
    for (int i = 0; i < KEYS_SIZE; i++)
    {
        squareKeys[i].pressed = false;
    }
    
}

void Synth_Open(void)
{
    initKeys();
    signalSum = 0;
    processNextDAC = false;

    SynthDAC_Open();
    SynthDAC_Write(dacMidLevel);

    SynthTimer_Open(updateDAC);
}


void Synth_Close(void)
{
    SynthTimer_Close();

    SynthDAC_Write(0);
    SynthDAC_Close();
}

void Synth_Press(Note note)
{
    squareKeys[note].pressed = true;
}

void Synth_Run(void)
{
    int signalSumBuffer = 0;
    bool isTimeForProcess = false;

    SynthTimer_DisableInterrupt();
    isTimeForProcess = processNextDAC;
    SynthTimer_EnableInterrupt();

    if (!isTimeForProcess) return;

    for (int i = 0; i < KEYS_SIZE; i++)
    {
        if (squareKeys[i].pressed == true)
        {
            signalSumBuffer += 12;
        }
    }

    if (signalSumBuffer > 127) signalSumBuffer = 127;
    if (signalSumBuffer < -127)signalSumBuffer = -127;

    SynthTimer_DisableInterrupt();
    signalSum = (signed char) signalSumBuffer;
    processNextDAC = false;
    SynthTimer_EnableInterrupt();
}