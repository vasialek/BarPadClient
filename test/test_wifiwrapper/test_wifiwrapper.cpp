#include "..\lib\mocks\mockBase64helper.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::AtLeast;

TEST()
{
    MockBase64Helper mockBase64Helper;
    EXPECT_CALL(mockBase64Helper, Encode()).Times(AtLeast(1));
}

// #include "unity.h"
// #include "wifiwrapper.h"

// void CanIsConnected()
// {

// }

// int main()
// {
//     UNITY_BEGIN();

//     RUN_TEST(CanIsConnected);

//     UNITY_END();
// }
