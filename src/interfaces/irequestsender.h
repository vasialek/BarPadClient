#pragma once

class IRequestSender
{
public:
    virtual void EnqueueWaiterRequest(const char *tableId, const char *requestId) = 0;
    virtual void EnqueueBillRequest(const char *tableId, const char *requestId) = 0;
    virtual void EnqueueCancelAllRequests(const char *tableId) = 0;
    virtual void ProcessRequests(unsigned long waitBeforeRetryMs) = 0;
};
