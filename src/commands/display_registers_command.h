#ifndef DISPLAY_REGISTERS_H
#define DISPLAY_REGISTERS_H

#include "command.h"

class DisplayRegistersCommand : public Command {
public:
    void execute(Debugger& debugger, const vector<string>& args) override;
};

#endif
