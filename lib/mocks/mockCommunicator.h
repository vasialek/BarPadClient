#pragma once

#include <cstring>
#include "interfaces\icommunicator.h"

class MockCommunicator : public ICommunicator
{
private:
    bool _nextRequestResult = true;

public:
    int RequestWaiterCalls = 0;
    int RequestBillCalls = 0;
    int CancelAllRequestsCalls = 0;

    char RequestIdWaiterCalled[33];
    char RequestIdBillCalled[33];

    char TableIdWaiterCalled[33];
    char TableIdBillCalled[33];
    char TableIdCancelAllCalled[33];

    MockCommunicator();
    virtual bool Heartbeat(const char *tableId) override;
    virtual bool RequestWaiter(const char *tableId, const char *requestId) override;
    virtual bool RequestBill(const char *tableId, const char *requestId) override;
    virtual bool CancelAllRequests(const char *tableId) override;
    void ResetTestResults();
    void SetNextRequestResult(bool result) { _nextRequestResult = result; }
    ~MockCommunicator();
};

MockCommunicator::MockCommunicator()
{
    ResetTestResults();
}

bool MockCommunicator::Heartbeat(const char *tableId)
{
    return _nextRequestResult;
}

bool MockCommunicator::RequestWaiter(const char *tableId, const char *requestId)
{
    RequestWaiterCalls++;
    strcpy(TableIdWaiterCalled, tableId);
    strcpy(RequestIdWaiterCalled, requestId);
    return _nextRequestResult;
}

bool MockCommunicator::RequestBill(const char *tableId, const char *requestId)
{
    RequestBillCalls++;
    strcpy(TableIdBillCalled, tableId);
    strcpy(RequestIdBillCalled, requestId);
    return _nextRequestResult;
}

bool MockCommunicator::CancelAllRequests(const char *tableId)
{
    CancelAllRequestsCalls++;
    strcpy(TableIdCancelAllCalled, tableId);
    return _nextRequestResult;
}

void MockCommunicator::ResetTestResults()
{
    RequestWaiterCalls = RequestBillCalls = CancelAllRequestsCalls = 0;

    // Clear table IDs
    TableIdWaiterCalled[0] = TableIdBillCalled[0] = TableIdCancelAllCalled[0] = 0;

    // Clear request IDs
    RequestIdWaiterCalled[0] = RequestIdBillCalled[0] = 0;
}

MockCommunicator::~MockCommunicator()
{
}
