#include "run_command.h"
#include "../core/debugger.h"
#include <sys/ptrace.h>
#include <iostream>

void RunCommand::execute(Debugger& debugger, const vector<string>& args) {
    if (debugger.is_running()) {
        std::cout << "Program is already running. Use 'continue' to resume." << std::endl;
        return;
    }
    debugger.set_running(true);
    ptrace(PTRACE_CONT, debugger.get_pid(), nullptr, nullptr);
    debugger.wait_for_signal();
}
