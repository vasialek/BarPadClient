#pragma once

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
    void ResetWaiterRequest();
public:
    RequestSender(ICommunicator *communicator);
    void EnqueueWaiterRequest(const char *requestId, const char *payload) override;
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
}

void RequestSender::EnqueueWaiterRequest(const char *requestId, const char *payload)
{
    strcpy(_waiterRequestIdToSend, requestId);
    strcpy(_waiterTableIdToSend, payload);
}

void RequestSender::ResetWaiterRequest()
{
    _waiterRequestIdToSend[0] = 0;
    _waiterTableIdToSend[0] = 0;
}

RequestSender::~RequestSender()
{
}

