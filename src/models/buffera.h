#pragma once

#include <cstring>
#include <iostream>

class BufferA
{
private:
    int _size = 0;
    char *_buffer = nullptr;
    void Delete();
public:
    BufferA();
    BufferA(const char *data);
    void Set(const char *source);
    const char *Get();
    ~BufferA();
};

BufferA::BufferA()
{
}

BufferA::BufferA(const char *data)
{
    Set(data);
}

const char *BufferA::Get()
{
    return _buffer;
}
void BufferA::Set(const char *source)
{
    if (source == nullptr)
    {
        Delete();
        return;
    }
    
    size_t len = strlen(source);
    if (len > _size)
    {
        Delete();
        _buffer = new char[len + 1];
        _size = len;
    }
    
    strcpy(_buffer, source);
}

void BufferA::Delete()
{
    if (_buffer != nullptr)
    {
        delete []_buffer;
        _buffer = nullptr;
        _size = 0;
    }
}

BufferA::~BufferA()
{
    Delete();
}
