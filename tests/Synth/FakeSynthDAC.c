#include "FakeSynthDAC.h"

static bool initialized = false;
static bool newWrite = false;
static unsigned char lastWriteValue = 0;

void SynthDAC_Open(void)
{
    initialized = true;
    newWrite = false;
    lastWriteValue = 0;
}

void SynthDAC_Close(void)
{
    initialized = false;
}

void SynthDAC_Write(unsigned char value)
{
    newWrite = true;
    lastWriteValue = value;
}

bool FakeSynthDAC_Initialized(void)
{
    return initialized;
}

bool FakeSynthDAC_GetLastWrite (unsigned char * writeValue)
{
    *writeValue = lastWriteValue;

    if (newWrite)
    {
        newWrite = false;
        return true;
    }
    return false;
}

void FakeSynthDAC_ClearLastWrite(void)
{
    lastWriteValue = 0;
    newWrite = false;
}