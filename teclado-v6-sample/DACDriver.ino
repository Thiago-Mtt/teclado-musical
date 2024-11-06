#include "SynthDAC.h"

#define DAC_PIN 25

void SynthDAC_Open(void)
{
  pinMode(DAC_PIN, OUTPUT);
}

void SynthDAC_Close(void)
{
  dacDisable(DAC_PIN);
  pinMode(DAC_PIN, INPUT);
}

void SynthDAC_Write(unsigned char value)
{
  dacWrite(DAC_PIN, value);
}