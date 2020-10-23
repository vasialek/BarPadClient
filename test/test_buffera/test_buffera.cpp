#include "unity.h"
#include "models\buffera.h"

void test_ctor()
{
    BufferA buffer("123");

    TEST_ASSERT_EQUAL_CHAR_ARRAY("123", buffer.Get(), 3);
}

void test_set_twice()
{
    BufferA buffer;
    buffer.Set("123");
    const char *expected = buffer.Get();
    
    buffer.Set("456");

    TEST_ASSERT_EQUAL(expected, buffer.Get());
}

void test_set_longer()
{
    BufferA buffer;
    buffer.Set("123");
    buffer.Set("123456");

    const char *actual = buffer.Get();

    TEST_ASSERT_EQUAL_CHAR_ARRAY("123456", actual, 6);
}

void test_set_null()
{
    BufferA buffer("123");

    buffer.Set(nullptr);

    TEST_ASSERT_NULL(buffer.Get());
}

void test_set_after_null()
{
    BufferA buffer("123");

    buffer.Set(nullptr);
    buffer.Set("12345678901234567890");

    TEST_ASSERT_EQUAL_CHAR_ARRAY("12345678901234567890", buffer.Get(), 20);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_ctor);
    RUN_TEST(test_set_twice);
    RUN_TEST(test_set_longer);
    RUN_TEST(test_set_null);
    RUN_TEST(test_set_after_null);

    UNITY_END();
}
