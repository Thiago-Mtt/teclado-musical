#include "ButtonHandler.h"
#include "ButtonReader.h"
#include "TimeService.h"

static ButtonState buttonStates[NUMBER_OF_BUTTONS];
static SoftTimer  debounceTimers[NUMBER_OF_BUTTONS];
static int        changedStates[NUMBER_OF_BUTTONS];

static bool initialized = false;

void ButtonHandler_Open (void)
{
    if (!initialized)
    {
        ButtonReader_Open();

        for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
        {
            buttonStates[i] = opened;
            changedStates[i] = 0;
            debounceTimers[i] = TimeService_CreateTimer(DEBOUNCE_TIME_US);
        }

        initialized = true;
    }
}

void ButtonHandler_Close (void)
{
    if (initialized)
    {
        ButtonReader_Close();
    
        if (initialized)
        for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
        {
            TimeService_DestroyTimer(debounceTimers[i]);
        }

        initialized = false;
    }
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

static bool isDebounceTimeOver (SoftTimer timer)
{
    return TimeService_CheckTimer(timer);
}

void ButtonHandler_Run (void)
{
    ButtonState state;

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        state = ButtonReader_Read(i);
        if (state == buttonStates[i])
        {
            TimeService_ResetTimer(debounceTimers[i]);
        }
        if (state != buttonStates[i] && isDebounceTimeOver(debounceTimers[i]))
        {
            buttonStates[i] = state;
            changedStates[i] = 1;
        }
    }
}