#include "quit_command.h"
#include "../core/debugger.h"
#include <cstdlib>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>

void QuitCommand::execute(Debugger& debugger, const vector<string>& args) {
    // Kill the child process
    kill(debugger.get_pid(), SIGKILL);
    int status;
    waitpid(debugger.get_pid(), &status, 0);
    std::cout << "Child process terminated." << std::endl;
    
    exit(0);
}
