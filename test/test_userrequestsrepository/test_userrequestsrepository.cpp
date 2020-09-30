#include <unity.h>
#include "userrequestsrepository.h"
#include "..\lib\mocks\mockRequestsender.h"
#include "..\lib\mocks\mockIdgenerator.h"

const char TableId[33] = "FakeTableIdForTest";

UserRequestsRepository *_repository = nullptr;
MockRequestSender _mockRequestSender;
MockIdGenerator _mockIdGenerator;

void setUp()
{
    // Unit test setup
    _repository = new UserRequestsRepository(TableId, &_mockRequestSender, &_mockIdGenerator);
}

void tearDown()
{
    delete _repository;
}

void test_requestwaiter_called()
{
    _mockRequestSender.ResetTestResults();
    _mockIdGenerator.SetNextResult("FakeRequestIdForWaiter");

    _repository->SetWaiterIsRequested();

    auto stat = _mockRequestSender.EnqueueWaiterRequestStat();
    TEST_ASSERT_EQUAL_INT32(1, stat.WasCalled());
    TEST_ASSERT_EQUAL_CHAR_ARRAY(TableId, stat.GetParam1(), strlen(TableId));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("FakeRequestIdForWaiter", stat.GetParam2(), strlen(stat.GetParam2()));
}

void test_requestbill_called()
{
    _mockRequestSender.ResetTestResults();
    
    _repository->SetBillIsRequested();

    // TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestBillCalls);
    // TEST_ASSERT_EQUAL_CHAR_ARRAY(TableId, _mockCommunicator.TableIdCalled, sizeof(TableId));
}

void test_requestwaiter_canceled()
{
    _mockRequestSender.ResetTestResults();
    const char *waiterRequestId = _repository->SetWaiterIsRequested();

    _repository->CancelRequest(waiterRequestId);

    // TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.CancelAllRequestsCalls);
    // TEST_ASSERT_EQUAL_CHAR_ARRAY(TableId, _mockCommunicator.TableIdCalled, sizeof(TableId));
}

void test_requestbill_is_canceled()
{
    _mockRequestSender.ResetTestResults();
    const char *billRequestId = _repository->SetBillIsRequested();

    _repository->CancelRequest(billRequestId);

    // TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.CancelAllRequestsCalls);
    // TEST_ASSERT_EQUAL_CHAR_ARRAY(TableId, _mockCommunicator.TableIdCalled, sizeof(TableId));
}

void test_repeat_when_requestwaiter_failed()
{
    
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_requestwaiter_called);
    // RUN_TEST(test_requestbill_called);
    // RUN_TEST(test_requestwaiter_canceled);
    // RUN_TEST(test_requestbill_is_canceled);

    UNITY_END();
}
