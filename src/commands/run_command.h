#ifndef RUN_COMMAND_H
#define RUN_COMMAND_H

#include "command.h"

class RunCommand : public Command {
public:
    void execute(Debugger& debugger, const vector<string>& args) override;
};

#endif
