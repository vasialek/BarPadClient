#pragma once

#include <iostream>
#include <cstring>
#include "interfaces\irequestsender.h"
#include "interfaces\icommunicator.h"
#include "interfaces\idatetimeprovider.h"

class RequestSender : public IRequestSender
{
private:
    ICommunicator *_communicator = nullptr;
    IDateTimeProvider *_dateTimeProvider = nullptr;
    // Store waiter request
    unsigned long _lastWaiterCallAt = -100000L;
    char _waiterTableIdToSend[33];
    char _waiterRequestIdToSend[33];
    // Store bill request
    unsigned long _lastBillCallAt = -100000L;
    char _billTableIdToSend[33];
    char _billRequestIdToSend[33];
    // Store cancel all request
    char _cancelAllTableIdToSend[33];
    
    void ResetWaiterRequest();
    void ResetBillRequest();
    void ResetCancelAllRequests();
    unsigned long GetEllapsedMsFromLastRun(unsigned long lastRunMs);
    bool IsWaiterDelayEnough(unsigned long waitBeforeRetryMs);
    bool IsBillDelayEnough(unsigned long waitBeforeRetryMs);
public:
    RequestSender(ICommunicator *communicator, IDateTimeProvider *dateTimeProvider);
    void EnqueueWaiterRequest(const char *tableId, const char *requestId) override;
    void EnqueueBillRequest(const char *tableId, const char *requestId) override;
    void EnqueueCancelAllRequests(const char *tableId) override;
    void ProcessRequests(unsigned long waitBeforeRetryMs) override;
    ~RequestSender();
};

RequestSender::RequestSender(ICommunicator *communicator, IDateTimeProvider *dateTimeProvider)
{
    _communicator = communicator;
    _dateTimeProvider = dateTimeProvider;
    ResetWaiterRequest();
}

void RequestSender::ProcessRequests(unsigned long waitBeforeRetryMs)
{
    if (_waiterRequestIdToSend[0] != 0)
    {
        if (IsWaiterDelayEnough(waitBeforeRetryMs))
        {
            _lastWaiterCallAt = _dateTimeProvider->millis();
            if (_communicator->RequestWaiter(_waiterTableIdToSend, _waiterRequestIdToSend))
            {
                ResetWaiterRequest();
            }
        }
    }

    // std::cout << "Is bill requested: " << (_billRequestIdToSend[0] != 0) << std::endl;
    if (_billRequestIdToSend[0] != 0)
    {
        if (IsBillDelayEnough(waitBeforeRetryMs))
        {
            _lastBillCallAt = _dateTimeProvider->millis();
            if (_communicator->RequestBill(_billTableIdToSend, _billRequestIdToSend))
            {
                ResetBillRequest();
            }
        }
    }

    if (_cancelAllTableIdToSend[0] != 0)
    {
        if (_communicator->CancelAllRequests(_cancelAllTableIdToSend))
        {
            ResetCancelAllRequests();
        }
        
    }
    
}

void RequestSender::EnqueueWaiterRequest(const char *tableId, const char *requestId)
{
    _lastWaiterCallAt = 0;
    strcpy(_waiterTableIdToSend, tableId);
    strcpy(_waiterRequestIdToSend, requestId);
}

void RequestSender::EnqueueBillRequest(const char *tableId, const char *requestId)
{
    strcpy(_billTableIdToSend, tableId);
    strcpy(_billRequestIdToSend, requestId);
}

void RequestSender::EnqueueCancelAllRequests(const char *tableId)
{
    strcpy(_cancelAllTableIdToSend, tableId);

    // Do not send request for waiter of bill
    ResetWaiterRequest();
    ResetBillRequest();
}

bool RequestSender::IsWaiterDelayEnough(unsigned long waitBeforeRetryMs)
{
    auto ellapsedFromLastRunMs = GetEllapsedMsFromLastRun(_lastWaiterCallAt);
    // std::cout << "Could run? " << ellapsedFromLastRunMs << " >= " << waitBeforeRetryMs << std::endl;
    return ellapsedFromLastRunMs >= waitBeforeRetryMs;
}

bool RequestSender::IsBillDelayEnough(unsigned long waitBeforeRetryMs)
{
    unsigned long ellapsedFromLastRunMs = GetEllapsedMsFromLastRun(_lastBillCallAt);
    return ellapsedFromLastRunMs >= waitBeforeRetryMs;
}

unsigned long RequestSender::GetEllapsedMsFromLastRun(unsigned long lastRunAtMs)
{
    unsigned long now = _dateTimeProvider->millis();
    // std::cout << "Last run at: " << lastRunAtMs << "ms. Now is: " << now << ". Ellapsed ms: " << (now - lastRunAtMs) << std::endl;
    return now - lastRunAtMs;
}

void RequestSender::ResetWaiterRequest()
{
    _waiterRequestIdToSend[0] = _waiterTableIdToSend[0] = 0;
}

void RequestSender::ResetBillRequest()
{
    _billRequestIdToSend[0] = _billTableIdToSend[0] = 0;
}

void RequestSender::ResetCancelAllRequests()
{
    _cancelAllTableIdToSend[0] = 0;
}

RequestSender::~RequestSender()
{
}

