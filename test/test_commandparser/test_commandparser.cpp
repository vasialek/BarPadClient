#include "unity.h"
#include "commandparser.h"

CommandParser _parser;

void test_setapihost_commandId()
{
    auto actual = _parser.Parse("/setapihost 123.456.789");

    TEST_ASSERT_NOT_NULL(actual);
    TEST_ASSERT_EQUAL_INT32(COMMAND_SET_API_HOST, actual->GetCommandId());
}

void test_setapihost_commandParam()
{
    auto actual = _parser.Parse("/setapihost 123.456.789");

    TEST_ASSERT_NOT_NULL(actual);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("123.456.789", actual->GetCommandParam(), 11);
}

void test_setapihost_commandParam_when_no_param()
{
    auto actual = _parser.Parse("/setapihost");

    TEST_ASSERT_NOT_NULL(actual);
    TEST_ASSERT_NULL(actual->GetCommandParam());
}

void test_parse_incorrect()
{
    auto actual = _parser.Parse("badcommand");

    TEST_ASSERT_NULL(actual);
}

int main()
{
    RUN_TEST(test_setapihost_commandId);
    RUN_TEST(test_setapihost_commandParam);
    RUN_TEST(test_setapihost_commandParam_when_no_param);
    RUN_TEST(test_parse_incorrect);
}
