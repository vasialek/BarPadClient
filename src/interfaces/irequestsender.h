#pragma once

class IRequestSender
{
public:
    virtual void EnqueueWaiterRequest(const char *requestId, const char *payload) = 0;
    virtual void EnqueueBillRequest(const char *requestId, const char *payload) = 0;
    virtual void ProcessRequests() = 0;
};
