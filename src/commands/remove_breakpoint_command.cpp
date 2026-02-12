#include "remove_breakpoint_command.h"
#include "../core/debugger.h"
#include <iostream>

void RemoveBreakpointCommand::execute(Debugger& debugger, const vector<string>& args) {
    if (args.empty()) {
        std::cerr << "Error: No address provided. Usage: delete <address>" << std::endl;
        return;
    }
    debugger.remove_breakpoint(stoul(args[0], nullptr, 16));
}
