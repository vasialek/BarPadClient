#ifndef LOGHELPER_H
#define LOGHELPER_H

// #define USE_SERIAL

#include <cstdio>
#include <ctime>
#include <cstring>
#ifndef USE_SERIAL
    #include <iostream>
#endif

class LogHelper
{
private:
    const static int MaxLogMessageLength = 247;
    const static int MessageOffset = 9;
    char _buf[MaxLogMessageLength + MessageOffset + 1];  // space to prepend HH:mm:ss
    void LogMessage(char* buf);
    void PrependTime();
public:
    void Info(const char* msg);
    void Info(const char* msg, int arg1);
};

void LogHelper::Info(const char* msg)
{
    PrependTime();
    strncpy(_buf + MessageOffset, (char *)msg, MaxLogMessageLength);
    LogMessage(_buf);
}

void LogHelper::Info(const char* fmt, int arg1)
{
    PrependTime();
    snprintf(_buf + MessageOffset, MaxLogMessageLength, fmt, arg1);
    LogMessage(_buf);
}

void LogHelper::PrependTime()
{
    auto t = time(0);
    auto now = localtime(&t);
    sprintf(_buf, "%02d:%02d:%02d  ", now->tm_hour, now->tm_min, now->tm_sec);
}

void LogHelper::LogMessage(char* buf)
{
    #ifdef USE_SERIAL
        Serial.println(buf);
    #else
        std::cout << buf << std::endl;
    #endif
}

#endif
