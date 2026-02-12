#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <cstdint>
#include <sys/types.h>

class Breakpoint {
private:
    uintptr_t address;
    pid_t pid;
    long original_data;
    bool enabled;

public:
    Breakpoint(uintptr_t address, pid_t pid);
    ~Breakpoint();
    void enable();
    void disable();
    void handle_hit();
    uintptr_t get_address() const { return address; }
    bool is_enabled() const { return enabled; }
};

#endif
