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
    // map from static (link-time) address to the breakpoint
    map<uintptr_t, unique_ptr<Breakpoint>> breakpoints;
    bool running;
    // runtime_addr = static_addr + load_bias (0 for non-PIE binaries)
    uintptr_t load_bias;

public:
    Debugger(pid_t pid);
    ~Debugger() = default;  // unique_ptr handles cleanup automatically!

    // Breakpoints. `addr` is always a static address -- the same one
    // nm/objdump/readelf would print -- never a raw runtime address.
    void remove_breakpoint(uintptr_t addr);
    void add_breakpoint(uintptr_t addr);

    // Get all breakpoint addresses (static, as supplied by the user)
    vector<uintptr_t> get_breakpoint_addresses();

    // Process control
    void wait_for_signal();

    // Getters
    pid_t get_pid() const { return child_pid; }
    bool is_running() const { return running; }
    uintptr_t get_load_bias() const { return load_bias; }

    // Setters
    void set_running(bool value) { running = value; }
    void set_load_bias(uintptr_t bias) { load_bias = bias; }
};

#endif
