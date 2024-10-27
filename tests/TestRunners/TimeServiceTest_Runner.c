#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(TimeService)
{
  RUN_TEST_CASE(TimeService, TimeIsZeroAfterCreate);
  RUN_TEST_CASE(TimeService, TimeIncreasedAfterSystemCallback);
  RUN_TEST_CASE(TimeService, TimeResetAfterDestroy);
  RUN_TEST_CASE(TimeService, TimeReadIsInterruptProof);
}

TEST_GROUP_RUNNER(TimeService_SoftTimer)
{
  RUN_TEST_CASE(TimeService_SoftTimer, CreateAndDestroySoftwareTimer);
  RUN_TEST_CASE(TimeService_SoftTimer, CheckTimerReturnsFalseWhenItsNotTime);
  RUN_TEST_CASE(TimeService_SoftTimer, CheckTimerReturnsTrueWhenItsTime)
  RUN_TEST_CASE(TimeService_SoftTimer, CheckTimerReturnsFalseAfterReturningTrueAndItsNotTimeYetAgain);
  RUN_TEST_CASE(TimeService_SoftTimer, CheckTimerReturnsTrueAfterAnotherPeriod);
  RUN_TEST_CASE(TimeService_SoftTimer, CheckTimerReturnsFalseBeforeTimeOverflows);
  RUN_TEST_CASE(TimeService_SoftTimer, CheckTimerReturnsTrueBeforeTimeOverflows);
  RUN_TEST_CASE(TimeService_SoftTimer, CheckTimerReturnsFalseAfterTimeOverflows);
  RUN_TEST_CASE(TimeService_SoftTimer, CheckTimerReturnsTrueAfterTimeOverflows);
  RUN_TEST_CASE(TimeService_SoftTimer, CheckTimerReturnsFalseAfterResetingTimer);
}