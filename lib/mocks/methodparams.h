#pragma once

#include <iostream>
#include <cstring>
#include "methodcallstat.h"

class MethodParams
{
private:
    char *_param1 = nullptr;
    char *_param2 = nullptr;
    char *_param3 = nullptr;
    int _methodCalled = 0;

    void DeleteParam1();
    void DeleteParam2();
    void DeleteParam3();
    void CopyTo(char **destination, const char *source);
public:
    MethodParams();

    void Reset();

    void IncreaseMethodCalled() { _methodCalled++; }

    MethodCallStat GetMethod() { return MethodCallStat(_methodCalled, _param1, _param2, _param3); }

    void SetParam1(const char *value);
    void SetParam2(const char *value);
    void SetParam3(const char *value);

    ~MethodParams();
};

MethodParams::MethodParams()
{
}

void MethodParams::SetParam1(const char *value)
{
    DeleteParam1();
    CopyTo(&_param1, value);
}

void MethodParams::SetParam2(const char *value)
{
    DeleteParam2();
    CopyTo(&_param2, value);
}

void MethodParams::SetParam3(const char *value)
{
    DeleteParam3();
    CopyTo(&_param3, value);
}

void MethodParams::CopyTo(char **destination, const char *source)
{
    if (source == nullptr)
    {
        return;
    }
    
    std::cout << "Setting param: `" << source << "`" << std::endl;
    size_t len = strlen(source);
    *destination = new char[len + 1];
    strcpy(*destination, source);
    (*destination)[len] = 0;
}

void MethodParams::DeleteParam1()
{
    if (_param1 != nullptr)
    {
        delete [] _param1;
        _param1 = nullptr;
    }
}

void MethodParams::DeleteParam2()
{
    if (_param2 != nullptr)
    {
        delete [] _param2;
        _param2 = nullptr;
    }
}

void MethodParams::DeleteParam3()
{
    if (_param3 != nullptr)
    {
        delete [] _param3;
        _param3 = nullptr;
    }
}

void MethodParams::Reset()
{
    DeleteParam1();
    DeleteParam2();
    DeleteParam3();

    _methodCalled = 0;
}

MethodParams::~MethodParams()
{
    Reset();
}
