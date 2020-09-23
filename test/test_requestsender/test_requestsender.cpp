#include "unity.h"
#include "requestsender.h"
#include "..\mocks\mockCommunicator.h"

const char TableId[33] = "FakeTableIdForTest";
const char WaiterRequestId[] = "WaiterRequestId";
const char BillRequestId[] = "BillRequestId";

MockCommunicator _mockCommunicator;
RequestSender _sender(&_mockCommunicator);

void test_nocalls_if_not_enqueued()
{
    _mockCommunicator.ResetTestResults();

    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(0, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_INT32(0, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_INT32(0, _mockCommunicator.CancelAllRequestsCalls);
}

// Waiter
void test_processrequests_for_waiter()
{
    _mockCommunicator.ResetTestResults();
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdWaiterCalled);
    TEST_ASSERT_EQUAL_STRING(WaiterRequestId, _mockCommunicator.RequestIdWaiterCalled);
}

void test_processrequests_waiter_once_if_ok()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(true);
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdWaiterCalled);
    TEST_ASSERT_EQUAL_STRING(WaiterRequestId, _mockCommunicator.RequestIdWaiterCalled);
}

void test_processrequests_waiter_repeat_on_error()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdWaiterCalled);
    TEST_ASSERT_EQUAL_STRING(WaiterRequestId, _mockCommunicator.RequestIdWaiterCalled);
}

// Bill request
void test_processrequests_for_bill()
{
    _mockCommunicator.ResetTestResults();
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdBillCalled);
    TEST_ASSERT_EQUAL_STRING(BillRequestId, _mockCommunicator.RequestIdBillCalled);
}

void test_processrequests_bill_once_if_ok()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(true);
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdBillCalled);
    TEST_ASSERT_EQUAL_STRING(BillRequestId, _mockCommunicator.RequestIdBillCalled);
}

void test_processrequests_bill_repeat_on_error()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdBillCalled);
    TEST_ASSERT_EQUAL_STRING(BillRequestId, _mockCommunicator.RequestIdBillCalled);
}

// Cancel requests
void test_processrequests_for_cancelallrequests()
{
    _mockCommunicator.ResetTestResults();
    _sender.EnqueueCancelAllRequests("TableIdForCancelAll");

    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.CancelAllRequestsCalls);
    TEST_ASSERT_EQUAL_STRING("TableIdForCancelAll", _mockCommunicator.TableIdCancelAllCalled);
}

void test_processrequests_cancelallrequests_once_if_ok()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(true);
    _sender.EnqueueCancelAllRequests(TableId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.CancelAllRequestsCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdCancelAllCalled);
}

void test_processrequests_cancelallrequests_repeat_on_error()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueCancelAllRequests(TableId);

    _sender.ProcessRequests();
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.CancelAllRequestsCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdCancelAllCalled);
}


void test_processrequests_noretries_after_cancellation()
{
    _mockCommunicator.ResetTestResults();
    _sender.EnqueueWaiterRequest(TableId, BillRequestId);
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.EnqueueCancelAllRequests(TableId);
    _sender.ProcessRequests();

    TEST_ASSERT_EQUAL_INT32(0, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_INT32(0, _mockCommunicator.RequestBillCalls);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_nocalls_if_not_enqueued);
    RUN_TEST(test_processrequests_noretries_after_cancellation);

    // Waiter
    RUN_TEST(test_processrequests_for_waiter);
    RUN_TEST(test_processrequests_waiter_once_if_ok);
    RUN_TEST(test_processrequests_waiter_repeat_on_error);

    // Bill
    RUN_TEST(test_processrequests_for_bill);
    RUN_TEST(test_processrequests_bill_once_if_ok);
    RUN_TEST(test_processrequests_bill_repeat_on_error);

    // Cancellation
    RUN_TEST(test_processrequests_for_cancelallrequests);
    RUN_TEST(test_processrequests_cancelallrequests_once_if_ok);
    RUN_TEST(test_processrequests_cancelallrequests_repeat_on_error);

    UNITY_END();
}
