/*
 *  Teclado Musical V1 - Onda quadrada
 *
 *
*/

#include "ButtonHandler.h"
#include "ButtonReader.h"
#include "Synth.h"

void setup() {
  // put your setup code here, to run once:
  ButtonState state;

  ButtonHandler_Open();
  Synth_Open();
  Synth_SetKeys(sampleSignal);

  state = ButtonReader_Read(0);
  if (state == pressed)
  {
    Synth_SetOperationType(fixedPoint);
  }
  else
  {
    Synth_SetOperationType(floatingPoint);
  }

}

void setKey(unsigned int keyNote, ButtonState state)
{
  if (state == opened)
  {
    Synth_Release((Note)keyNote);
  }
  else if (state == pressed)
  {
    Synth_Press((Note)keyNote);
  }
}

void loop() {
  bool changedState;
  ButtonState state;
  /*
  ButtonHandler_Run();

  for (unsigned int keyNote = 0; keyNote < NUMBER_OF_BUTTONS; keyNote++)
  {
    changedState = ButtonHandler_GetButtonState(keyNote, &state);
    if (changedState)
    {
      Serial.println(keyNote);
    }
  }
  */

  //changedState = ButtonHandler_GetButtonState(0, &state);
  //Serial.println((int)state, DEC);
  

  ButtonHandler_Run();
  for (unsigned int keyNote = 0; keyNote < NUMBER_OF_BUTTONS; keyNote++)
  {
    changedState = ButtonHandler_GetButtonState(keyNote, &state);
    if (changedState)
    {
      setKey(keyNote, state);
    }
  }

  Synth_Run();


}
