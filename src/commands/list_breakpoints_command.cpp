#include "list_breakpoints_command.h"
#include "../core/debugger.h"
#include <iostream>

void ListBreakpointsCommand::execute(Debugger& debugger, const vector<string>& args) {
    vector<uintptr_t> breakpoints = debugger.get_breakpoint_addresses();
    
    if (breakpoints.empty()) {
        cout << "No breakpoints set." << endl;
    } else {
        cout << "Breakpoints:" << endl;
        for (const uintptr_t& addr : breakpoints) {
            cout << "  0x" << hex << addr << endl;
        }
    }
}
