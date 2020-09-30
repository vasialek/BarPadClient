#pragma once

#include <cstring>
#include <iostream>

class MethodCallStat
{
    private:
        int _wasCalled = 0;
        char *_param1 = nullptr;
        char *_param2 = nullptr;
        char *_param3 = nullptr;
        void CopyTo(char **destination, const char *source);
        const char *GetNullableParam(char *param);
    public:
        MethodCallStat(int wasCalled, const char *param1, const char *param2, const char *param3);
        int WasCalled() { return _wasCalled; }
        const char *GetParam1() { return _param1; }
        const char *GetParam2() { return _param2; }
        const char *GetParam3() { return _param3; }
        void Print();
        ~MethodCallStat();
};

MethodCallStat::MethodCallStat(int wasCalled, const char *param1, const char *param2, const char *param3)
{
    CopyTo(&_param1, param1);
    CopyTo(&_param2, param2);
    CopyTo(&_param3, param3);
    _wasCalled = wasCalled;
}

void MethodCallStat::Print()
{
    std::cout << "Method was called " << _wasCalled << " time(s)." << std::endl;
    std::cout << "\tparam 1: `" << (_param1 == nullptr ? "<NULL>" : _param1) << "`" << std::endl;
    std::cout << "\tparam 2: `" << (_param2 == nullptr ? "<NULL>" : _param2) << "`" << std::endl;
    std::cout << "\tparam 3: `" << (_param3 == nullptr ? "<NULL>" : _param3) << "`" << std::endl;
}

void MethodCallStat::CopyTo(char **destination, const char *source)
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

MethodCallStat::~MethodCallStat()
{
    if (_param1 != nullptr)
    {
        delete [] _param1;
        _param1 = nullptr;
    }
    if (_param2 != nullptr)
    {
        delete [] _param2;
        _param2 = nullptr;
    }
    if (_param3 != nullptr)
    {
        delete [] _param3;
        _param3 = nullptr;
    }
}
