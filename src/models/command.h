#pragma once

#include "models\buffera.h"

class Command
{
private:
    int _commandId = -1;
    BufferA *_commandParam = nullptr;
public:
    Command(int commandId);
    Command(int commandId, const char *param);
    int GetCommandId();
    const char *GetCommandParam();
    ~Command();
};

Command::Command(int commandId)
{
    _commandId = commandId;
}

Command::Command(int commandId, const char *param)
{
    _commandId = commandId;
    _commandParam = new BufferA(param);
}

int Command::GetCommandId()
{
    return _commandId;
}

const char *Command::GetCommandParam()
{
    return _commandParam == nullptr ? nullptr : _commandParam->Get();
}

Command::~Command()
{
    if (_commandParam != nullptr)
    {
        delete _commandParam;
    }
}
