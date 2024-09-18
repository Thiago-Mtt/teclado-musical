#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(ButtonHandler)
{
  RUN_TEST_CASE(ButtonHandler, OpenAndClose);
  RUN_TEST_CASE(ButtonHandler, InitializeButtonReader);
  RUN_TEST_CASE(ButtonHandler, CloseButtonReaderOnClose);
  RUN_TEST_CASE(ButtonHandler, ButtonsInitializedAsOpen);
  RUN_TEST_CASE(ButtonHandler, ButtonsOutOfRangeReadErrorValue);
  RUN_TEST_CASE(ButtonHandler, ButtonPressedChangesStateToPressed);
  RUN_TEST_CASE(ButtonHandler, ButtonReleaseChangesStateToOpen);
}