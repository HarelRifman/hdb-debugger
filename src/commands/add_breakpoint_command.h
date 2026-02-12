#ifndef ADD_BREAKPOINT_COMMAND_H
#define ADD_BREAKPOINT_COMMAND_H

#include "command.h"

class AddBreakpointCommand : public Command {
public:
    void execute(Debugger& debugger, const vector<string>& args) override;
};

#endif
