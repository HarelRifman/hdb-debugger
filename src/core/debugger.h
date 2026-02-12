#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <cstdint>
#include <map>
#include <memory>
#include <sys/types.h>
#include <vector>
#include "breakpoint.h"

using namespace std;

class Debugger {
private:
    pid_t child_pid;
    // map from address to the breakpoint
    map<uintptr_t, unique_ptr<Breakpoint>> breakpoints;
    bool running;

public:
    Debugger(pid_t pid);
    ~Debugger() = default;  // unique_ptr handles cleanup automatically!

    // Breakpoints
    void remove_breakpoint(uintptr_t addr);
    void add_breakpoint(uintptr_t addr);

    // Get all breakpoint addresses
    vector<uintptr_t> get_breakpoint_addresses();

    // Process control
    void wait_for_signal();
    
    // Getters
    pid_t get_pid() const { return child_pid; }
    bool is_running() const { return running; }    

    // Setters
    void set_running(bool value) { running = value; }
};

#endif
