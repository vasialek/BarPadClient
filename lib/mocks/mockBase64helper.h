#include "..\.pio\libdeps\native\googletest\googlemock\include\gmock\gmock.h"
#include "..\src\helpers\base64helper.h"

class MockBase64Helper : public Base64Helper
{
private:
    /* data */
public:
    MockBase64Helper(/* args */);
    ~MockBase64Helper();
};

MockBase64Helper::MockBase64Helper(/* args */)
{
}

MockBase64Helper::~MockBase64Helper()
{
}
