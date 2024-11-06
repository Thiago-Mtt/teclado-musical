#include "SynthTimer.h"

hw_timer_t *timer = NULL;

static void (*timerCallback)(void);

void ARDUINO_ISR_ATTR onTimer() {
  timerCallback();
}

void SynthTimer_Open(void (*callBack)(void))
{
  timerCallback = callBack;

  // Set timer frequency to 1Mhz
  timer = timerBegin(1000000);
  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer);

  // Set alarm to call onTimer function every ~1/32KHz seconds (value in microseconds).
  // Repeat the alarm (third parameter) with unlimited count = 0 (fourth parameter).
  timerAlarm(timer, 31, true, 0);
}

void SynthTimer_Close(void){}

void SynthTimer_DisableInterrupt(void)
{
  noInterrupts();
}
void SynthTimer_EnableInterrupt(void)
{
  interrupts();
}

