#include "unity.h"
#include "cryptoservice.h"

const char PlainMessage[] = "Plain text...";

CryptoService _service;

void CanEncrypt()
{
    auto actual = _service.Encrypt(PlainMessage, "123");
    std::cout << "Encrpypted: `" << actual << "`\n";

    TEST_ASSERT_NOT_NULL(actual);
    TEST_ASSERT_EQUAL_STRING("A8Kcw57Dq8KLVHDCnl0X", actual);
}

void EncryptDecrypt()
{
    auto encrypted = _service.Encrypt(PlainMessage, "123");
    auto actual = _service.Decrypt(encrypted, "123");

    TEST_ASSERT_EQUAL_STRING(PlainMessage, actual);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(CanEncrypt);
    RUN_TEST(EncryptDecrypt);

    UNITY_END();
}