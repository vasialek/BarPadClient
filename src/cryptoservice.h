#include <cstring>
#include "settings.h"
#include "models\buffera.h"
// #include <iostream>

class CryptoService
{
private:
    char _buffer[MaxPayloadLength];
public:
    const char *Encrypt(const char *input, const char *secret);
    const char *Decrypt(const char *input, const char *secret);
};

const char *CryptoService::Encrypt(const char *input, const char *secret)
{
    auto len = strlen(input);
    auto secretLen = strlen(secret);

    for (size_t i = 0; i < len; i++)
    {
        _buffer[i] = input[len - i - 1];
    }
    _buffer[len] = 0;

    return _buffer;
}

const char *CryptoService::Decrypt(const char *input, const char *secret)
{
    auto len = strlen(input);
    char buffer[len + 1];
    auto secretLen = strlen(secret);

    for (size_t i = 0; i < len; i++)
    {
        buffer[i] = input[len - i - 1];
    }
    buffer[len] = 0;
    
    strcpy(_buffer, buffer);

    return _buffer;
}
