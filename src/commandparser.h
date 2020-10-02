#pragma once

#include <iostream>
#include "interfaces\icommandparser.h"
#include "models\command.h"

#define COMMAND_SET_API_HOST        1

class CommandParser : public ICommandParser
{
private:
    Command *_command = nullptr;
    const char *GetParam(const char *cmd, int offset);
public:
    Command *Parse(const char *cmd) override;
    ~CommandParser();
};

Command *CommandParser::Parse(const char *cmd)
{
    char *p = (char *)cmd;
    if (strncmp(cmd, "/setapihost", 11) == 0)
    {
        const char *param = GetParam(cmd, 11);
        _command = new Command(COMMAND_SET_API_HOST, param);
        return _command;
    }
    
    return nullptr;
}

const char *CommandParser::GetParam(const char *cmd, int offset)
{
    int len = strlen(cmd);
    // std::cout << "Command is `" << cmd << "`, length is: " << len << ", offset is: " << offset << std::endl;
    if (offset >= len)
    {
        return nullptr;
    }
    
    while (offset < len && isspace(cmd[offset]))
    {
        offset++;
    }
    
    return cmd + offset;
}

CommandParser::~CommandParser()
{
    if (_command != nullptr)
    {
        delete _command;
    }
}
