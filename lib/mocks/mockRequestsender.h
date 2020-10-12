#pragma once

#include "interfaces\irequestsender.h"
#include "methodparams.h"

class MockRequestSender : public IRequestSender
{
private:
    MethodParams _paramsForEnqueueWaiter;
    MethodParams _paramsForEnqueueBill;
public:
    MockRequestSender();

    MethodCallStat EnqueueWaiterRequestStat();
    MethodCallStat EnqueueBillRequestStat();

    const char *GetEnqueueWaiterRequestParamTableId();

    void EnqueueWaiterRequest(const char *tableId, const char *requestId);
    void EnqueueBillRequest(const char *tableId, const char *requestId) override;
    void EnqueueCancelAllRequests(const char *tableId) override;
    void ProcessRequests(unsigned long waitBeforeRetryMs) override;

    void ResetTestResults()
    {
        _paramsForEnqueueWaiter.Reset();
    }

    ~MockRequestSender();
};

MockRequestSender::MockRequestSender()
{
}

void MockRequestSender::EnqueueWaiterRequest(const char *tableId, const char *requestId)
{
    _paramsForEnqueueWaiter.SetParam1(tableId);
    _paramsForEnqueueWaiter.SetParam2(requestId);
    _paramsForEnqueueWaiter.IncreaseMethodCalled();
};

void MockRequestSender::EnqueueBillRequest(const char *tableId, const char *requestId)
{
    _paramsForEnqueueBill.SetParam1(tableId);
    _paramsForEnqueueBill.SetParam2(requestId);
    _paramsForEnqueueBill.IncreaseMethodCalled();
}

void MockRequestSender::EnqueueCancelAllRequests(const char *tableId)
{

}

void MockRequestSender::ProcessRequests(unsigned long waitBeforeRetryMs)
{

}

MethodCallStat MockRequestSender::EnqueueWaiterRequestStat()
{
    return _paramsForEnqueueWaiter.GetMethod();
}

MethodCallStat MockRequestSender::EnqueueBillRequestStat()
{
    return _paramsForEnqueueBill.GetMethod();
}

const char *MockRequestSender::GetEnqueueWaiterRequestParamTableId()
{
    return "XXXXXXXXXXXXXXXX";
    // return _paramsForEnqueueWaiter.GetParam1();
}

MockRequestSender::~MockRequestSender()
{
}
