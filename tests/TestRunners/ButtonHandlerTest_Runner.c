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
  RUN_TEST_CASE(ButtonHandler, ManyButtonsPressedChangesStatesToPressed);
  RUN_TEST_CASE(ButtonHandler, ManyButtonsPressedThenReleasedChangesStatesToOpen);
  RUN_TEST_CASE(ButtonHandler, InitializedButtonIsNotChanged);
  RUN_TEST_CASE(ButtonHandler, DetectButtonChangeFromOpenToPressed);
  RUN_TEST_CASE(ButtonHandler, ReturnNoChangeAfterSecondButtonReadCall);
  RUN_TEST_CASE(ButtonHandler, DetectButtonChangeFromPressedToOpen);
  RUN_TEST_CASE(ButtonHandler, ReturnNoChangeAfterSecondButtonOpenRead);
  RUN_TEST_CASE(ButtonHandler, DetectChangeFromMultipleButtons);
}