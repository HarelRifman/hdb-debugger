#ifndef LIST_BREAKPOINTS_COMMAND_H
#define LIST_BREAKPOINTS_COMMAND_H

#include "command.h"

class ListBreakpointsCommand : public Command {
public:
    void execute(Debugger& debugger, const vector<string>& args) override;
};

#endif
