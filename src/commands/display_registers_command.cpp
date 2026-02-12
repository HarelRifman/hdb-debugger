#include "display_registers_command.h"
#include "../core/debugger.h"
#include <sys/ptrace.h>
#include <sys/user.h>
#include <iostream>

void DisplayRegistersCommand::execute(Debugger& debugger, const vector<string>& args) {
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, debugger.get_pid(), nullptr, &regs);
    
    std::cout << "Registers:" << std::endl;
    std::cout << "  rax: 0x" << std::hex << regs.rax << std::endl;
    std::cout << "  rbx: 0x" << std::hex << regs.rbx << std::endl;
    std::cout << "  rcx: 0x" << std::hex << regs.rcx << std::endl;
    std::cout << "  rdx: 0x" << std::hex << regs.rdx << std::endl;
    std::cout << "  rsi: 0x" << regs.rsi << std::endl;
    std::cout << "  rdi: 0x" << regs.rdi << std::endl;
    std::cout << "  rbp: 0x" << regs.rbp << std::endl;
    std::cout << "  rsp: 0x" << regs.rsp << std::endl;
    std::cout << "  rip: 0x" << regs.rip << std::endl;
}
