#include "debugger.h"
#include <iostream>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>

using namespace std;

Debugger::Debugger(pid_t pid) : child_pid(pid), running(false) {}

// Note: No manual cleanup needed! unique_ptr handles it automatically

/*
    The function waits for signal from the child process.
    it checks if the child process finished, was stopped by a signal or by a breakpoint
    and handles the signal accordingly
*/
void Debugger::wait_for_signal() {
    int status;
    waitpid(child_pid, &status, 0);

    // Check if child process exited normally (called exit() or returned from main)
    if (WIFEXITED(status)) {
        cout << "Program exited with code " << WEXITSTATUS(status) << endl;
        running = false;
        return;
    }

    // Check if child was stopped by a signal (not exited, not terminated)
    if (!WIFSTOPPED(status)) {
        return;
    }

    // Check which signal stopped the child (5 = SIGTRAP, sent by breakpoint/INT3)
    int sig = WSTOPSIG(status);
    if (sig != 5) {
        return;
    }

    // We got SIGTRAP - check if we hit one of our breakpoints
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, child_pid, nullptr, &regs);
    
    // RIP points to instruction AFTER INT3, so subtract 1 to get breakpoint address
    uintptr_t bp_addr = regs.rip - 1;
    if (breakpoints.count(bp_addr) > 0) {
        cout << "Breakpoint hit at 0x" << hex << bp_addr << endl;
        breakpoints[bp_addr]->handle_hit();
    }
}

void Debugger::remove_breakpoint(uintptr_t addr) {
    if (breakpoints.count(addr) == 0) {
        cout << "No breakpoint at 0x" << hex << addr << endl;
        return;
    }
    breakpoints[addr]->disable();
    breakpoints.erase(addr);
}

void Debugger::add_breakpoint(uintptr_t addr) {
    if (breakpoints.count(addr) > 0) {
        cout << "Breakpoint already exists at 0x" << hex << addr << endl;
        return;
    }
    breakpoints[addr] = make_unique<Breakpoint>(addr, child_pid);
    breakpoints[addr]->enable();
}

vector<uintptr_t> Debugger::get_breakpoint_addresses() {
    vector<uintptr_t> addresses;
    for (const auto& [addr, bp] : breakpoints) {
        addresses.push_back(addr);
    }
    return addresses;
}