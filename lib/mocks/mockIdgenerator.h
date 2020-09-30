#pragma once

#include <cstring>
#include "interfaces\iidgenerator.h"

class MockIdGenerator : public IIdGenerator
{
private:
    char _nextGeneratedId[64] = "FakeGeneratedId";
public:
    void SetNextResult(const char *id);
    const char *Generate() override;
};

void MockIdGenerator::SetNextResult(const char *id)
{
    strcpy(_nextGeneratedId, id);
}

const char *MockIdGenerator::Generate()
{
    return _nextGeneratedId;
}
