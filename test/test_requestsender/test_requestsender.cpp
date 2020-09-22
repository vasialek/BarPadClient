#include "unity.h"
#include "requestsender.h"
#include "..\mocks\mockCommunicator.h"

const char TableId[33] = "FakeTableIdForTest";
const char WaiterRequestId[] = "WaiterRequestId";
const char BillRequestId[] = "BillRequestId";

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

// Bill request
void test_processrequests_for_bill()
{
    _mockCommunicator.ResetTestResults();
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_STRING(BillRequestId, _mockCommunicator.TableIdCalled);
}

void test_processrequests_bill_once_if_ok()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(true);
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_STRING(BillRequestId, _mockCommunicator.TableIdCalled);
}

void test_processrequests_bill_repeat_on_error()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_STRING(BillRequestId, _mockCommunicator.TableIdCalled);
}

int main()
{
    UNITY_BEGIN();

    // Waiter
    RUN_TEST(test_processrequests_for_waiter);
    RUN_TEST(test_processrequests_waiter_once_if_ok);
    RUN_TEST(test_processrequests_waiter_repeat_on_error);

    // Bill
    RUN_TEST(test_processrequests_for_bill);
    RUN_TEST(test_processrequests_bill_once_if_ok);
    RUN_TEST(test_processrequests_bill_repeat_on_error);

    UNITY_END();
}
