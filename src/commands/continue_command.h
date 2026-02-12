#ifndef CONTINUE_COMMAND_H
#define CONTINUE_COMMAND_H

#include "command.h"

class ContinueCommand : public Command {
public:
    void execute(Debugger& debugger, const vector<string>& args) override;
};

#endif
