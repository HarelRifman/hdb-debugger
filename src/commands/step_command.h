#ifndef STEP_COMMAND_H
#define STEP_COMMAND_H

#include "command.h"

class StepCommand : public Command {
public:
    void execute(Debugger& debugger, const vector<string>& args) override;
};

#endif
