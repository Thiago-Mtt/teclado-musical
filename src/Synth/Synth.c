#include "Synth.h"
#include "SynthDAC.h"
#include "SynthTimer.h"

#include <stdbool.h>

const unsigned char dacMidLevel = 128;

//static bool keysPressed[8];

static void updateDAC (void)
{
    SynthDAC_Write(dacMidLevel);
}

void Synth_Open(void)
{
    SynthDAC_Open();
    SynthDAC_Write(dacMidLevel);

    SynthTimer_Open(updateDAC);
}


void Synth_Close(void)
{
    SynthDAC_Close();
    SynthTimer_Close();
}