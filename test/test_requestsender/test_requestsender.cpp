#include "unity.h"
#include "requestsender.h"
#include "..\mocks\mockCommunicator.h"

const char TableId[33] = "FakeTableIdForTest";
const char WaiterRequestId[] = "WaiterRequestId";

MockCommunicator _mockCommunicator;
RequestSender _sender(&_mockCommunicator);

void test_processrequests_for_waiter()
{
    _mockCommunicator.ResetTestResults();
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_STRING(WaiterRequestId, _mockCommunicator.TableIdCalled);
}

void test_processrequests_waiter_once_if_ok()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(true);
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_STRING(WaiterRequestId, _mockCommunicator.TableIdCalled);
}

void test_processrequests_waiter_repeat_on_error()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_STRING(WaiterRequestId, _mockCommunicator.TableIdCalled);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_processrequests_for_waiter);
    RUN_TEST(test_processrequests_waiter_once_if_ok);
    RUN_TEST(test_processrequests_waiter_repeat_on_error);

    UNITY_END();
}
