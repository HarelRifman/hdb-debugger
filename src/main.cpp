#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <unistd.h>

#include "core/debugger.h"
#include "registry/command_registry.h"
#include "registry/command_factory.h"

using namespace std;

vector<string> split(const string& line) {
    vector<string> tokens;
    istringstream iss(line);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

bool file_exists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

void run_target(char* program, char* argv[]) {
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execvp(program, argv);
    perror("execvp failed");
    exit(1);
}

void run_debugger(Debugger& debugger, CommandRegistry& registry) {
    int status;
    waitpid(debugger.get_pid(), &status, 0);
    cout << "Debugger attached. Type 'help' for commands." << endl;

    string line;
    while (true) {
        cout << "hdb> ";
        if (!getline(cin, line)) {
            break;
        }

        vector<string> tokens = split(line);
        if (tokens.empty()) {
            continue;
        }

        string cmd = tokens[0];
        vector<string> args(tokens.begin() + 1, tokens.end());
        registry.execute(cmd, debugger, args);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <program>" << endl;
        return 1;
    }

    if (!file_exists(argv[1])) {
        cerr << "Error: File '" << argv[1] << "' does not exist" << endl;
        return 1;
    }

    pid_t child_pid = fork();
    if (child_pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (child_pid == 0) {
        run_target(argv[1], argv + 1);
    }

    // Parent process
    Debugger debugger(child_pid);
    CommandRegistry registry;
    
    // Factory creates all commands - registry owns them
    CommandFactory::create_and_register(registry);
    
    run_debugger(debugger, registry);

    return 0;
}
