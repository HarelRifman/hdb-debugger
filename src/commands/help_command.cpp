#include "help_command.h"
#include <iostream>

void HelpCommand::execute(Debugger& debugger, const vector<string>& args) {
    std::cout << "Commands:" << std::endl;
    std::cout << "  break <addr>   (b)  - Set breakpoint at address" << std::endl;
    std::cout << "  delete <addr>  (d)  - Remove breakpoint at address" << std::endl;
    std::cout << "  list           (l)  - List all breakpoints" << std::endl;
    std::cout << "  run            (r)  - Start/run the program" << std::endl;
    std::cout << "  continue       (c)  - Continue execution" << std::endl;
    std::cout << "  step           (s)  - Single step one instruction" << std::endl;
    std::cout << "  regs                - Display registers" << std::endl;
    std::cout << "  quit           (q)  - Exit debugger" << std::endl;
}
