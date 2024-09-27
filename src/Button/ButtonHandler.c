#include "ButtonHandler.h"
#include "ButtonReader.h"

static ButtonState buttonStates[NUMBER_OF_BUTTONS];
static int        changedStates[NUMBER_OF_BUTTONS];

void ButtonHandler_Open (void)
{
    ButtonReader_Open();

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        buttonStates[i] = opened;
        changedStates[i] = 0;
    }
    
}

void ButtonHandler_Close (void)
{
    ButtonReader_Close();
}

int ButtonHandler_GetButtonState (unsigned int button, ButtonState * state)
{
    int changed = 0;
    if (button >= NUMBER_OF_BUTTONS)
    {
        *state = errorState;
        return 0;
    }

    *state = buttonStates[button];
    
    changed = changedStates[button];
    changedStates[button] = 0;

    return changed;
}

void ButtonHandler_Run (void)
{
    ButtonState state;

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        state = ButtonReader_Read(i);
        if (state != buttonStates[i])
        {
            buttonStates[i] = state;
            changedStates[i] = 1;
        }
    }
}