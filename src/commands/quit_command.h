#ifndef QUIT_COMMAND_H
#define QUIT_COMMAND_H

#include "command.h"

class QuitCommand : public Command {
public:
    void execute(Debugger& debugger, const vector<string>& args) override;
};

#endif
