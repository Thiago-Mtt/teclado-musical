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
  RUN_TEST_CASE(Synth, ReleaseC4KeyReturnsDACtoMidLevel);
  RUN_TEST_CASE(Synth, ReleaseAndPressKeyResetsSignalProgression);
  RUN_TEST_CASE(Synth, KeepKeyPressedRepeatsSignalOnDAC);
  RUN_TEST_CASE(Synth, PressD4);
  RUN_TEST_CASE(Synth, PressE4);
  RUN_TEST_CASE(Synth, PressF4);
  RUN_TEST_CASE(Synth, PressG4);
  RUN_TEST_CASE(Synth, PressA4);
  RUN_TEST_CASE(Synth, PressB4);
  RUN_TEST_CASE(Synth, PressC5);
  RUN_TEST_CASE(Synth, PressTwoKeysSumsTheirSignals);
  RUN_TEST_CASE(Synth, PressTwoKeysAndReleasingOneRemovesOnlyOneKey);
  RUN_TEST_CASE(Synth, CheckSampleKeyValueC4);
  RUN_TEST_CASE(Synth, CheckSampleyKeyAmplitudeC4);
  RUN_TEST_CASE(Synth, PrintSampleSignalKeyTypeC4);
  RUN_TEST_CASE(Synth, CheckSampleyKeyAmplitudeADSRC4);
}