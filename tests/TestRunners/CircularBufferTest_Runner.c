#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(CircularBuffer)
{
  RUN_TEST_CASE(CircularBuffer, CreateAndDestroy);
  RUN_TEST_CASE(CircularBuffer, BufferReturnsFalseWhenReadingWhileEmpty);
  RUN_TEST_CASE(CircularBuffer, ReturnNullIfCapacityIsZero);
  RUN_TEST_CASE(CircularBuffer, BufferIsEmptyWhenCreated);
  RUN_TEST_CASE(CircularBuffer, WriteAndRead);
  RUN_TEST_CASE(CircularBuffer, WriteAndReadTwice);
  RUN_TEST_CASE(CircularBuffer, BuffertNotEmptyAfterWrite);
  RUN_TEST_CASE(CircularBuffer, BuffertEmptyAfterWriteAndRead);
  RUN_TEST_CASE(CircularBuffer, IsBufferFull);
  RUN_TEST_CASE(CircularBuffer, BufferIsEmptyAfterFull);
  RUN_TEST_CASE(CircularBuffer, IntegrityAfterCreate);
  RUN_TEST_CASE(CircularBuffer, IntegrityWhenFull);
  RUN_TEST_CASE(CircularBuffer, IntegrityAfterWrapAround);
  RUN_TEST_CASE(CircularBuffer, WrapAround);
}
