#pragma once

#include "models\command.h"

class ICommandParser
{
public:
    virtual Command *Parse(const char *cmd) = 0;
};
