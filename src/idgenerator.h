#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>

class IdGenerator
{
private:
    char *_buf;
    char GetRandomSymbol();
    int GetRandom(int max);
public:
    static const int LengthOfId = 32;
    IdGenerator();
    ~IdGenerator();
    const char* Generate();
};

IdGenerator::IdGenerator()
{
    _buf = new char[IdGenerator::LengthOfId + 1];
    srand(time(0));
}

const char* IdGenerator::Generate()
{
    for (int i = 0; i < IdGenerator::LengthOfId; i++)
    {
        _buf[i] = IdGenerator::GetRandomSymbol();
    }
    _buf[IdGenerator::LengthOfId] = '\0';
    return _buf;
}

char IdGenerator::GetRandomSymbol()
{
    const char symbols[] {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm', 'n', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    static int totalSymbols = sizeof(symbols);
    int random = IdGenerator::GetRandom(totalSymbols);
    return symbols[random];
}

int IdGenerator::GetRandom(int max)
{
    return rand() % max;
}

IdGenerator::~IdGenerator()
{
    delete []_buf;
}

#endif
