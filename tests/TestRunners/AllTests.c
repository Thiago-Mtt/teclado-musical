#include "unity_fixture.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(CircularBuffer);
  RUN_TEST_GROUP(TimeService);
  RUN_TEST_GROUP(TimeService_SoftTimer);
  RUN_TEST_GROUP(ButtonHandler);
  RUN_TEST_GROUP(Synth);
}

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}