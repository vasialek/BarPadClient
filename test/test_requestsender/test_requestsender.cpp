#include "unity.h"
#include "requestsender.h"
#include "..\mocks\mockCommunicator.h"
#include "..\mocks\mockDatetimeprovider.h"

const unsigned long WaitBeforRetryMs = 10000;
unsigned long _lastMillis = 10000;

const char TableId[33] = "FakeTableIdForTest";
const char WaiterRequestId[] = "WaiterRequestId";
const char BillRequestId[] = "BillRequestId";

MockCommunicator _mockCommunicator;
MockDateTimeProvider _mockDateTimeProvider;
RequestSender _sender(&_mockCommunicator, &_mockDateTimeProvider);

unsigned long GetAndIncreaseMillis()
{
    unsigned long millis = _lastMillis;
    _lastMillis += WaitBeforRetryMs;
    return millis;
}

void test_nocalls_if_not_enqueued()
{
    _mockCommunicator.ResetTestResults();

    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(0, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_INT32(0, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_INT32(0, _mockCommunicator.CancelAllRequestsCalls);
}

void test_waiter_noretry_when_delay_too_short()
{
    unsigned long firstRunMs = GetAndIncreaseMillis();
    unsigned long secondRunMs = firstRunMs + WaitBeforRetryMs - 1;
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);
    _mockDateTimeProvider.SetNextMillis(firstRunMs);
    _sender.ProcessRequests(WaitBeforRetryMs);
    
    _mockDateTimeProvider.SetNextMillis(secondRunMs);
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestWaiterCalls);
}

// Waiter
void test_processrequests_for_waiter()
{
    _mockCommunicator.ResetTestResults();
    _mockDateTimeProvider.SetNextMillis(GetAndIncreaseMillis());
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdWaiterCalled);
    TEST_ASSERT_EQUAL_STRING(WaiterRequestId, _mockCommunicator.RequestIdWaiterCalled);
}

void test_processrequests_waiter_once_if_ok()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(true);
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _sender.ProcessRequests(WaitBeforRetryMs);
    _mockDateTimeProvider.SetNextMillis(GetAndIncreaseMillis());
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdWaiterCalled);
    TEST_ASSERT_EQUAL_STRING(WaiterRequestId, _mockCommunicator.RequestIdWaiterCalled);
}

void test_processrequests_waiter_repeat_on_error()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _mockDateTimeProvider.SetNextMillis(GetAndIncreaseMillis());
    _sender.ProcessRequests(WaitBeforRetryMs);
    _mockDateTimeProvider.SetNextMillis(GetAndIncreaseMillis());
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdWaiterCalled);
    TEST_ASSERT_EQUAL_STRING(WaiterRequestId, _mockCommunicator.RequestIdWaiterCalled);
}

void test_waiter_newcall_without_delay()
{
    _mockCommunicator.ResetTestResults();
    _mockDateTimeProvider.SetNextMillis(WaitBeforRetryMs);
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);

    _sender.ProcessRequests(WaitBeforRetryMs);
    _sender.EnqueueWaiterRequest(TableId, WaiterRequestId);
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.RequestWaiterCalls);
}

// Bill request
void test_processrequests_for_bill()
{
    _mockCommunicator.ResetTestResults();
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdBillCalled);
    TEST_ASSERT_EQUAL_STRING(BillRequestId, _mockCommunicator.RequestIdBillCalled);
}

void test_processrequests_bill_once_if_ok()
{
    _mockCommunicator.ResetTestResults();
    _mockDateTimeProvider.SetNextMillis(0);
    _mockCommunicator.SetNextRequestResult(true);
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.ProcessRequests(WaitBeforRetryMs);
    _mockDateTimeProvider.SetNextMillis(WaitBeforRetryMs);
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdBillCalled);
    TEST_ASSERT_EQUAL_STRING(BillRequestId, _mockCommunicator.RequestIdBillCalled);
}

void test_processrequests_bill_repeat_on_error()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _mockDateTimeProvider.SetNextMillis(GetAndIncreaseMillis());
    _sender.ProcessRequests(WaitBeforRetryMs);
    _mockDateTimeProvider.SetNextMillis(WaitBeforRetryMs);
    _sender.ProcessRequests(GetAndIncreaseMillis());

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdBillCalled);
    TEST_ASSERT_EQUAL_STRING(BillRequestId, _mockCommunicator.RequestIdBillCalled);
}

void test_bill_noretry_when_delay_too_short()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueBillRequest(TableId, BillRequestId);
    _mockDateTimeProvider.SetNextMillis(0);
    _sender.ProcessRequests(WaitBeforRetryMs);
   
    _mockDateTimeProvider.SetNextMillis(WaitBeforRetryMs - 1);
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestBillCalls);
}

void test_bill_newcall_without_delay()
{
    _mockCommunicator.ResetTestResults();
    _mockDateTimeProvider.SetNextMillis(WaitBeforRetryMs);
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.ProcessRequests(WaitBeforRetryMs);
    _sender.EnqueueBillRequest(TableId, BillRequestId);
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.RequestBillCalls);
}

// Cancel requests
void test_processrequests_for_cancelallrequests()
{
    _mockCommunicator.ResetTestResults();
    _sender.EnqueueCancelAllRequests("TableIdForCancelAll");

    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.CancelAllRequestsCalls);
    TEST_ASSERT_EQUAL_STRING("TableIdForCancelAll", _mockCommunicator.TableIdCancelAllCalled);
}

void test_processrequests_cancelallrequests_once_if_ok()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(true);
    _sender.EnqueueCancelAllRequests(TableId);

    _sender.ProcessRequests(WaitBeforRetryMs);
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.CancelAllRequestsCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdCancelAllCalled);
}

void test_processrequests_cancelallrequests_repeat_on_error()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueCancelAllRequests(TableId);

    _mockDateTimeProvider.SetNextMillis(0);
    _sender.ProcessRequests(WaitBeforRetryMs);
    _mockDateTimeProvider.SetNextMillis(WaitBeforRetryMs);
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.CancelAllRequestsCalls);
    TEST_ASSERT_EQUAL_STRING(TableId, _mockCommunicator.TableIdCancelAllCalled);
}

void test_cancelallrequests_noretry_when_delay_too_short()
{
    _mockCommunicator.ResetTestResults();
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueCancelAllRequests(TableId);
    _mockDateTimeProvider.SetNextMillis(0);
    _sender.ProcessRequests(WaitBeforRetryMs);
   
    _mockDateTimeProvider.SetNextMillis(WaitBeforRetryMs - 1);
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.CancelAllRequestsCalls);
}

void test_cancelallrequess_newcall_without_delay()
{
    _mockCommunicator.ResetTestResults();
    _mockDateTimeProvider.SetNextMillis(WaitBeforRetryMs);
    _mockCommunicator.SetNextRequestResult(false);
    _sender.EnqueueCancelAllRequests(TableId);

    _sender.ProcessRequests(WaitBeforRetryMs);
    _sender.EnqueueCancelAllRequests(TableId);
    _sender.ProcessRequests(WaitBeforRetryMs);

    TEST_ASSERT_EQUAL_INT32(2, _mockCommunicator.CancelAllRequestsCalls);
}

void test_processrequests_noretries_after_cancellation()
{
    _mockCommunicator.ResetTestResults();
    _sender.EnqueueWaiterRequest(TableId, BillRequestId);
    _sender.EnqueueBillRequest(TableId, BillRequestId);

    _sender.EnqueueCancelAllRequests(TableId);
    _sender.ProcessRequests(WaitBeforRetryMs);

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
    RUN_TEST(test_waiter_noretry_when_delay_too_short);
    RUN_TEST(test_waiter_newcall_without_delay);

    // Bill
    RUN_TEST(test_processrequests_for_bill);
    RUN_TEST(test_processrequests_bill_once_if_ok);
    RUN_TEST(test_processrequests_bill_repeat_on_error);
    RUN_TEST(test_bill_noretry_when_delay_too_short);
    RUN_TEST(test_bill_newcall_without_delay);

    // Cancellation
    RUN_TEST(test_processrequests_for_cancelallrequests);
    RUN_TEST(test_processrequests_cancelallrequests_once_if_ok);
    RUN_TEST(test_processrequests_cancelallrequests_repeat_on_error);
    RUN_TEST(test_cancelallrequests_noretry_when_delay_too_short);
    RUN_TEST(test_cancelallrequess_newcall_without_delay);

    UNITY_END();
}
