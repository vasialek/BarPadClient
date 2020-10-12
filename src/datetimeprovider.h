#pragma once

#include "interfaces\idatetimeprovider.h"

class DateTimeProvider : public IDateTimeProvider
{
private:
    unsigned long millis() override;
};

unsigned long DateTimeProvider::millis()
{
    #ifdef ARDUINO
        return millis();
    #else
        return 0;
    #endif
}
