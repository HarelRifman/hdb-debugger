#include <sys/ptrace.h>
#include <sys/user.h>
#include <iostream>
#include "breakpoint.h"
#include <sys/wait.h>
#include <cstdint>
#include <cerrno>
#include <cstring>

using namespace std;

Breakpoint::Breakpoint(uintptr_t address, pid_t pid) 
    : address(address), pid(pid), original_data(0), enabled(false) {}

Breakpoint::~Breakpoint() {
    if (enabled) {
        disable();
    }
}

/*
    Set breakpoint, save the original data and replace the first byte with 0xCC 
    which is the INT3 instruction to stop the program execution when reached.
*/
void Breakpoint::enable() {
    if (enabled) {
        return;
    }
    
    errno = 0;
    original_data = ptrace(PTRACE_PEEKDATA, pid, (void*)address, nullptr);
    if (errno != 0) {
        cerr << "Error: Failed to read memory at 0x" << hex << address 
             << ": " << strerror(errno) << endl;
        return;
    }
    
    long data_with_int3 = (original_data & ~0xFF) | 0xCC;
    
    errno = 0;
    if (ptrace(PTRACE_POKEDATA, pid, (void*)address, (void*)data_with_int3) == -1 && errno != 0) {
        cerr << "Error: Failed to write breakpoint at 0x" << hex << address 
             << ": " << strerror(errno) << endl;
        return;
    }
    
    enabled = true;
}

/*
    Remove breakpoint, restore the original data and remove the INT3 instruction.
*/
void Breakpoint::disable() {
    if (!enabled) {
        return;
    }
    ptrace(PTRACE_POKEDATA, pid, (void*)address, (void*)original_data);
    enabled = false;
}

/*
    After hitting breakpoint, restore instruction and rewind instruction pointer (RIP) to the original instruction.
*/
void Breakpoint::handle_hit() {
    disable();
    
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
    
    // Rewind instruction pointer (RIP points AFTER the INT3)
    regs.rip -= 1;
    ptrace(PTRACE_SETREGS, pid, nullptr, &regs);

    // Single-step the original instruction
    ptrace(PTRACE_SINGLESTEP, pid, nullptr, nullptr);
    int status;
    waitpid(pid, &status, 0);
    
    // Re-enable the breakpoint
    enable();
}
