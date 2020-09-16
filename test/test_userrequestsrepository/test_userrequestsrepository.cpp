#include <unity.h>
#include "userrequestsrepository.h"
#include "..\lib\mocks\mockCommunicator.h"

const char TableId[33] = "FakeTableIdForTest";

UserRequestsRepository *_repository = nullptr;
MockCommunicator _mockCommunicator;

void setUp()
{
    // Unit test setup
    _repository = new UserRequestsRepository(TableId, &_mockCommunicator);
}

void tearDown()
{
    delete _repository;
}

void test_requestwaiter_called()
{
    _mockCommunicator.ResetTestResults();

    _repository->SetWaiterIsRequested();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestWaiterCalls);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(TableId, _mockCommunicator.TableIdCalled, sizeof(TableId));
}

void test_requestbill_called()
{
    _mockCommunicator.ResetTestResults();
    
    _repository->SetBillIsRequested();

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.RequestBillCalls);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(TableId, _mockCommunicator.TableIdCalled, sizeof(TableId));
}

void test_requestwaiter_canceled()
{
    _mockCommunicator.ResetTestResults();
    const char *waiterRequestId = _repository->SetWaiterIsRequested();

    _repository->CancelRequest(waiterRequestId);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.CancelAllRequestsCalls);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(TableId, _mockCommunicator.TableIdCalled, sizeof(TableId));
}

void test_requestbill_is_canceled()
{
    _mockCommunicator.ResetTestResults();
    const char *billRequestId = _repository->SetBillIsRequested();

    _repository->CancelRequest(billRequestId);

    TEST_ASSERT_EQUAL_INT32(1, _mockCommunicator.CancelAllRequestsCalls);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(TableId, _mockCommunicator.TableIdCalled, sizeof(TableId));
}

void test_repeat_when_requestwaiter_failed()
{
    
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_requestwaiter_called);
    RUN_TEST(test_requestbill_called);
    RUN_TEST(test_requestwaiter_canceled);
    RUN_TEST(test_requestbill_is_canceled);

    UNITY_END();
}
