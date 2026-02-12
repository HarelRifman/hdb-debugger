#include "command_registry.h"
#include <iostream>

using namespace std;

void CommandRegistry::register_command(const vector<string>& names, unique_ptr<Command> cmd) {
    // Get raw pointer before moving
    Command* raw = cmd.get();
        
    // Registry takes ownership
    owned_commands.push_back(std::move(cmd));
    
    // Map all aliases to the same command
    for (const string& name : names) {
        commands[name] = raw;
    }
}

bool CommandRegistry::execute(const string& name, Debugger& debugger, const vector<string>& args) {
    if (commands.count(name) == 0) {
        cout << "Unknown command: " << name << ". Type 'help' for available commands." << endl;
        return false;
    }
    
    commands[name]->execute(debugger, args);
    return true;
}
