#pragma once

#include <iostream>
#include <cstring>
#include <cstdio>
#include "interfaces\irequestbuilder.h"

class RequestBuilder : public IRequestBuilder
{
private:
    // Format heartbeat request
    int _sizeOfHeartbeat = -1;
    const char *_heartbeatFmt = "{\"tableid\":\"%s\"}";
    char *_heartbeatBuffer = nullptr;
    // Format bill/waiter requests payload
    int _sizeOfRequest = -1;
    const char *_requestFmt = "{\"tableid\":\"%s\",\"requestid\":\"%s\",\"item\":\"%s\"}";
    char *_requestBuffer = nullptr;

public:
    RequestBuilder();
    const char *BuildHeartbeat(const char *tableId) override;
    const char *BuildWaiterRequest(const char *tableId, const char *requestId) override;
    const char *BuildBillRequest(const char *tableId, const char *requestId) override;
    ~RequestBuilder();
};

RequestBuilder::RequestBuilder()
{
    // Space to insert 2 UID - table (BarPad), request ID and type of request (bill/waiter)
    _sizeOfRequest = strlen(_requestFmt) + 2 * 32 + 10;
    _requestBuffer = new char[_sizeOfRequest + 1];
    // Space to insert table ID (BarPad)
    _sizeOfHeartbeat = strlen(_heartbeatFmt) + 32;
    _heartbeatBuffer = new char[_sizeOfHeartbeat + 1];
}

const char *RequestBuilder::BuildHeartbeat(const char *tableId)
{
    snprintf(_heartbeatBuffer, _sizeOfHeartbeat, _heartbeatFmt, tableId);
    return _heartbeatBuffer;
}

const char *RequestBuilder::BuildWaiterRequest(const char *tableId, const char *requestId)
{
    snprintf(_requestBuffer, _sizeOfRequest, _requestFmt, tableId, requestId, "waiter");
    return _requestBuffer;
}

const char *RequestBuilder::BuildBillRequest(const char *tableId, const char *requestId)
{
    snprintf(_requestBuffer, _sizeOfRequest, _requestFmt, tableId, requestId, "bill");
    return _requestBuffer;
}

RequestBuilder::~RequestBuilder()
{
    if (_heartbeatBuffer != nullptr)
    {
        delete[] _heartbeatBuffer;
    }
    if (_requestBuffer != nullptr)
    {
        delete[] _requestBuffer;
    }
}
