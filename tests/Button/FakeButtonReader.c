#include "FakeButtonReader.h"
#include "ButtonHandler.h"

static int initialized = 0;
static ButtonState buttonStates[NUMBER_OF_BUTTONS];

void ButtonReader_Open(void)
{
    initialized = 1;

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        buttonStates[i] = open;
    }
}

void ButtonReader_Close(void)
{
    initialized = 0;
}

ButtonState ButtonReader_Read(unsigned int button)
{
    return buttonStates[button];
}


int FakeButtonReader_IsInitialized(void)
{
    return initialized;
}

void FakeButtonReader_PressButton(unsigned int button)
{
    buttonStates[button] = pressed;
}

void FakeButtonReader_ReleaseButton(unsigned int button)
{
    buttonStates[button] = open;
}