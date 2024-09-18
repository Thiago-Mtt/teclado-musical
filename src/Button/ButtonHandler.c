#include "ButtonHandler.h"
#include "ButtonReader.h"

static ButtonState buttonStates[NUMBER_OF_BUTTONS];

void ButtonHandler_Open (void)
{
    ButtonReader_Open();

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        buttonStates[i] = open;
    }
    
}

void ButtonHandler_Close (void)
{
    ButtonReader_Close();
}

int ButtonHandler_GetButtonState (unsigned int button, ButtonState * state)
{
    if (button >= NUMBER_OF_BUTTONS)
    {
        *state = error;
        return 0;
    }

    *state = buttonStates[button];

    return 0;
}

void ButtonHandler_Run (void)
{
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        buttonStates[i] = ButtonReader_Read(i);
    }
}