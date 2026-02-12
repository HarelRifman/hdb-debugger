#include "add_breakpoint_command.h"
#include "../core/debugger.h"
#include <iostream>

void AddBreakpointCommand::execute(Debugger& debugger, const vector<string>& args) {
    if (args.empty()) {
        cerr << "Error: No address provided. Usage: break <address>" << endl;
        return;
    }
    debugger.add_breakpoint(stoul(args[0], nullptr, 16));
}
