#ifndef REMOVE_BREAKPOINT_COMMAND_H
#define REMOVE_BREAKPOINT_COMMAND_H

#include "command.h"

class RemoveBreakpointCommand : public Command {
public:
    void execute(Debugger& debugger, const vector<string>& args) override;
};

#endif
