#ifndef HELP_COMMAND_H
#define HELP_COMMAND_H

#include "command.h"

class HelpCommand : public Command {
public:
    void execute(Debugger& debugger, const vector<string>& args) override;
};

#endif
