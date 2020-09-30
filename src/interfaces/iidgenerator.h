#pragma once

class IIdGenerator
{
public:
    virtual const char* Generate() = 0;
};
