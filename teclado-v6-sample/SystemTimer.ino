#include "SystemTimer.h"
#include "TimeTypes.h"

typedef unsigned long timeMicroseconds;
typedef void (*Callback)(timeMicroseconds useconds);

hw_timer_t * hardwareSystemTimer;

Callback storedSystemTimerCallback;

void ARDUINO_ISR_ATTR onSystemTimer() {
  storedSystemTimerCallback(10000);
}

void SystemTimer_Create(void (*timerCallback)(unsigned long))
{
  storedSystemTimerCallback = timerCallback;
  // Set timer frequency to 1Mhz
  hardwareSystemTimer =  timerBegin(1000);
  // Attach onTimer function to our timer.
  timerAttachInterrupt(hardwareSystemTimer, &onSystemTimer);

  // Set alarm to call onTimer function every 10 milliseconds.
  // Repeat the alarm (third parameter) with unlimited count = 0 (fourth parameter).
  timerAlarm(hardwareSystemTimer, 10, true, 0);
}
void SystemTimer_Destroy(void)
{
  timerEnd(hardwareSystemTimer);
}

void SystemTimer_Pause(void)
{
  noInterrupts();
}
void SystemTimer_Continue(void)
{
  interrupts();
}
