#pragma once

class IDateTimeProvider
{
public:
    virtual unsigned long millis() = 0;
};
