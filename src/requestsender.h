#pragma once

#include <iostream>
#include <cstring>
#include "interfaces\irequestsender.h"
#include "interfaces\icommunicator.h"

class RequestSender : public IRequestSender
{
private:
    ICommunicator *_communicator = nullptr;
    // Store waiter request
    char _waiterTableIdToSend[33];
    char _waiterRequestIdToSend[33];
    // Store bill request
    char _billTableIdToSend[33];
    char _billRequestIdToSend[33];
    // Store cancel all request
    char _cancelAllTableIdToSend[33];
    
    void ResetWaiterRequest();
    void ResetBillRequest();
    void ResetCancelAllRequests();
public:
    RequestSender(ICommunicator *communicator);
    void EnqueueWaiterRequest(const char *tableId, const char *requestId) override;
    void EnqueueBillRequest(const char *tableId, const char *requestId) override;
    void EnqueueCancelAllRequests(const char *tableId) override;
    void ProcessRequests() override;
    ~RequestSender();
};

RequestSender::RequestSender(ICommunicator *communicator)
{
    _communicator = communicator;
    ResetWaiterRequest();
}

void RequestSender::ProcessRequests()
{
    if (_waiterRequestIdToSend[0] != 0)
    {
        if (_communicator->RequestWaiter(_waiterTableIdToSend, _waiterRequestIdToSend))
        {
            ResetWaiterRequest();
        }
    }

    if (_billRequestIdToSend[0] != 0)
    {
        if (_communicator->RequestBill(_billTableIdToSend, _billRequestIdToSend))
        {
            ResetBillRequest();
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

