#pragma once

#include <iostream>
#include "interfaces\idatetimeprovider.h"

class MockDateTimeProvider : public IDateTimeProvider
{
private:
    unsigned long _nexMillis = 0;
public:
    unsigned long millis() override;
    void SetNextMillis(unsigned long millis);
};

unsigned long MockDateTimeProvider::millis()
{
    // std::cout << "millis: " << _nexMillis << std::endl;
    return _nexMillis;
}

void MockDateTimeProvider::SetNextMillis(unsigned long millis)
{
    _nexMillis = millis;
}
