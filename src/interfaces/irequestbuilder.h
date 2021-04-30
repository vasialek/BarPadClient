#pragma once

class IRequestBuilder
{
public:
    virtual const char *BuildHeartbeat(const char *tableId) = 0;
    virtual const char *BuildHeartbeat(const char *tableId, const char *payload) = 0;
    virtual const char *BuildWaiterRequest(const char *tableId, const char *requestId) = 0;
    virtual const char *BuildBillRequest(const char *tableId, const char *requestId) = 0;
};
