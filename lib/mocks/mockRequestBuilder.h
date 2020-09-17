#pragma once

#include <cstdio>
#include "interfaces\irequestbuilder.h"

class MockRequestBuilder : public IRequestBuilder
{
private:
public:
    char TableIdCalled[64];
    MockRequestBuilder();
    const char *BuildHeartbeat(const char *tableId) override;
    const char *BuildWaiterRequest(const char *tableId, const char *requestId) override;
    const char *BuildBillRequest(const char *tableId, const char *requestId) override;
    void ResetTestResult();
    ~MockRequestBuilder();
};

MockRequestBuilder::MockRequestBuilder()
{
}

const char *MockRequestBuilder::BuildHeartbeat(const char *tableId)
{
    sprintf(TableIdCalled, tableId);
    return "tableId";
}

void MockRequestBuilder::ResetTestResult()
{
    TableIdCalled[0] = 0;
}

MockRequestBuilder::~MockRequestBuilder()
{
}
