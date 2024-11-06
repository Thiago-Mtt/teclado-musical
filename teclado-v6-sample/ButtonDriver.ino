#include "ButtonReader.h"
#include "ButtonState.h"


/*
typedef static enum
{ C4, D4, E4, F4, G4, A4, B4, C5, KEYS_SIZE}
Note;
*/

/* Mapeamento dos pinos */
uint8_t static botoes[] = {15, 2, 0, 4, 16, 17, 5, 18};

void ButtonReader_Open(void)
{
  for (uint8_t i = 0; i < KEYS_SIZE; i++)
  {
    pinMode(botoes[i], INPUT_PULLUP);
  }
}

void ButtonReader_Close(void){}

ButtonState ButtonReader_Read(unsigned int button)
{
  ButtonState state;
  if (button >= KEYS_SIZE) return errorState;

  if(digitalRead(botoes[button]) == LOW) return pressed;
  else return opened;
  
}