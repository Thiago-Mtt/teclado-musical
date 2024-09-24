#include "Synth.h"
#include "SynthDAC.h"
#include "SynthTimer.h"

#include <stdbool.h>

typedef struct 
{
    bool pressed;
    unsigned int period;
    unsigned int counter;
    signed char  amplitude;
}SquareWaveKey;


const unsigned char dacMidLevel = 128;
const double samplingFrequency  = 32000;

const double notesFrequencies[KEYS_SIZE] = 
{   261.63, /*C4*/
    293.66, /*D4*/
    329.63, /*E4*/
    349.23, /*F4*/
    392.00, /*G4*/
    440.00, /*A4*/
    466.16, /*B4*/
    493.88  /*C5*/
};

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
        squareKeys[i].counter = 0;
        squareKeys[i].period = (unsigned int)(samplingFrequency/notesFrequencies[i]);
        squareKeys[i].amplitude = 0;
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

void Synth_Release(Note note)
{
    squareKeys[note].pressed = false;
}


static void processKeys (void)
{
    for (int i = 0; i < KEYS_SIZE; i++)
    {
        if(!squareKeys[i].pressed)
        {
            squareKeys[i].counter = 0;
            squareKeys[i].amplitude = 0;
            continue;
        } 

        if (squareKeys[i].counter >= squareKeys[i].period/2)
        {
            squareKeys[i].amplitude = -12;
        }
        else squareKeys[i].amplitude = 12;

        squareKeys[i].counter++;
        squareKeys[i].counter = squareKeys[i].counter % squareKeys[i].period;
    }
}

void Synth_Run(void)
{
    int signalSumBuffer = 0;
    bool isTimeForProcess = false;

    SynthTimer_DisableInterrupt();
    isTimeForProcess = processNextDAC;
    SynthTimer_EnableInterrupt();

    if (!isTimeForProcess) return;

    processKeys();

    for (int i = 0; i < KEYS_SIZE; i++)
    {
        signalSumBuffer += squareKeys[i].amplitude;
    }

    if (signalSumBuffer > 127) signalSumBuffer = 127;
    if (signalSumBuffer < -127)signalSumBuffer = -127;

    SynthTimer_DisableInterrupt();
    signalSum = (signed char) signalSumBuffer;
    processNextDAC = false;
    SynthTimer_EnableInterrupt();
}