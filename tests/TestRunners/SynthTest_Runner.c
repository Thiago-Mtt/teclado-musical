#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(Synth)
{
  RUN_TEST_CASE(Synth, OpenAndClose);
  RUN_TEST_CASE(Synth, DACInitialization);
  RUN_TEST_CASE(Synth, TimerInitialization);
  RUN_TEST_CASE(Synth, DACWriteAfterInitialization);
  RUN_TEST_CASE(Synth, DACDetectNewWrite);
  RUN_TEST_CASE(Synth, DACClearLastWrite);
  RUN_TEST_CASE(Synth, DACWriteMidLevelWhileNoPressedKeys);
  RUN_TEST_CASE(Synth, PressC4IncrementsDACBy12);
  RUN_TEST_CASE(Synth, PauseTimerInterruptOnCheckingIfIsTimeToProcessNextDAC);
  RUN_TEST_CASE(Synth, PauseTimerInterruptOnNextDACValueUpdate);
  RUN_TEST_CASE(Synth, ProcessNextDACValueOnlyAfterDACWrite);
  RUN_TEST_CASE(Synth, PressC4IncrementsDACBy12ForHalfPeriod);
  RUN_TEST_CASE(Synth, PressC4DecrementDACBy12AfterHalfPeriod);
}