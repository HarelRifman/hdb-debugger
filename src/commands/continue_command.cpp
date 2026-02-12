#include "continue_command.h"
#include "../core/debugger.h"
#include <sys/ptrace.h>
#include <iostream>

void ContinueCommand::execute(Debugger& debugger, const vector<string>& args) {
    if (!debugger.is_running()) {
        std::cout << "Program is not running. Use 'run' to start." << std::endl;
        return;
    }
    ptrace(PTRACE_CONT, debugger.get_pid(), nullptr, nullptr);
    debugger.wait_for_signal();
}
