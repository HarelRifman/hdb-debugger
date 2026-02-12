#include "step_command.h"
#include "../core/debugger.h"
#include <sys/ptrace.h>
#include <iostream>

void StepCommand::execute(Debugger& debugger, const vector<string>& args) {
    if (!debugger.is_running()) {
        std::cout << "Program is not running. Use 'run' to start." << std::endl;
        return;
    }
    ptrace(PTRACE_SINGLESTEP, debugger.get_pid(), nullptr, nullptr);
    debugger.wait_for_signal();
}
