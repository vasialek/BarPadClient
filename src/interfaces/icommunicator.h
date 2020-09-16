#ifndef ICOMMUNICATOR_H
#define ICOMMUNICATOR_H

class ICommunicator
{
public:
    virtual bool Heartbeat(const char *tableId) = 0;
    virtual bool RequestWaiter(const char *tableId, const char *requestId) = 0;
    virtual bool RequestBill(const char *tableId, const char *requestId) = 0;
    virtual bool CancelAllRequests(const char *tableId) = 0;
    virtual ~ICommunicator() = default;
};

#endif
