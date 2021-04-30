#include "unity.h"
#include "requestbuilder.h"

const char TableId[33] = "FakeTableIdForTest";
const char Payload[] = "This is long payload to store some telemetry";

RequestBuilder _builder;

void TEST_ASSERT_CONTAINS(const char *actual, const char *expected);

void test_buildheartbeat()
{
    const char *request = _builder.BuildHeartbeat(TableId);

    TEST_ASSERT_NOT_NULL(request);
    TEST_ASSERT_EQUAL_STRING("{\"tableid\":\"FakeTableIdForTest\",\"payload\":\"\"}", request);
}

void test_buildheartbeat_whenpayload()
{
    const char *request = _builder.BuildHeartbeat(TableId, Payload);

    TEST_ASSERT_NOT_NULL(request);
    TEST_ASSERT_CONTAINS("\"payload\":", request);
    TEST_ASSERT_CONTAINS(Payload, request);
}

void test_buildwaiterrequest()
{
    const char *request = _builder.BuildWaiterRequest(TableId, "WaiterRequestId");

    TEST_ASSERT_NOT_NULL(request);
    TEST_ASSERT_EQUAL_STRING("{\"tableid\":\"FakeTableIdForTest\",\"requestid\":\"WaiterRequestId\",\"item\":\"waiter\"}", request);
}

void test_buildbillrequest()
{
    const char *request = _builder.BuildBillRequest(TableId, "BillRequestId");

    TEST_ASSERT_NOT_NULL(request);
    TEST_ASSERT_EQUAL_STRING("{\"tableid\":\"FakeTableIdForTest\",\"requestid\":\"BillRequestId\",\"item\":\"bill\"}", request);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_buildheartbeat);
    RUN_TEST(test_buildheartbeat_whenpayload);
    RUN_TEST(test_buildwaiterrequest);
    RUN_TEST(test_buildbillrequest);

    UNITY_END();
}

void TEST_ASSERT_CONTAINS(const char *expected, const char *actual)
{
    if (actual == nullptr)
    {
        TEST_FAIL_MESSAGE("Actual string is NULL.");
        return;
    }
    if (!strstr(actual, expected))
    {
        std::cout << "Actual string does not contain expected:" << std::endl;
        std::cout << "Actual:   " << actual << std::endl;
        std::cout << "Expected: " << expected << std::endl;
        TEST_FAIL();
    }
}
