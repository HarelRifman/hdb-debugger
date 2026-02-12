#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "../commands/command.h"

using namespace std;

class CommandRegistry {
private:
    vector<unique_ptr<Command>> owned_commands;  // Owns the commands
    map<string, Command*> commands;              // Maps names to commands (aliases)

public:
    // Register a command with one or more names (aliases) - takes ownership
    void register_command(const vector<string>& names, unique_ptr<Command> cmd);
    
    // Execute a command by name
    bool execute(const string& name, Debugger& debugger, const vector<string>& args);
};

#endif
