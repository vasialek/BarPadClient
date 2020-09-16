#pragma once

#include "interfaces\icommunicator.h"

class MockCommunicator : public ICommunicator
{
private:
    bool _nextRequestResult = true;
public:
    int RequestWaiterCalls = 0;
    int RequestBillCalls = 0;
    int CancelAllRequestsCalls = 0;
    char TableIdCalled[33];
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
    strcpy(TableIdCalled, tableId);
    return _nextRequestResult;
}

bool MockCommunicator::RequestBill(const char *tableId, const char *requestId)
{
    RequestBillCalls++;
    strcpy(TableIdCalled, tableId);
    return _nextRequestResult;
}

bool MockCommunicator::CancelAllRequests(const char *tableId)
{
    CancelAllRequestsCalls++;
    strcpy(TableIdCalled, tableId);
    return _nextRequestResult;
}

void MockCommunicator::ResetTestResults()
{
    RequestWaiterCalls = RequestBillCalls = CancelAllRequestsCalls = 0;
    TableIdCalled[0] = 0;
}

MockCommunicator::~MockCommunicator()
{
}

