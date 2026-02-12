#include "command_factory.h"
#include "../commands/step_command.h"
#include "../commands/continue_command.h"
#include "../commands/display_registers_command.h"
#include "../commands/add_breakpoint_command.h"
#include "../commands/remove_breakpoint_command.h"
#include "../commands/run_command.h"
#include "../commands/list_breakpoints_command.h"
#include "../commands/help_command.h"
#include "../commands/quit_command.h"

using namespace std;

void CommandFactory::create_and_register(CommandRegistry& registry) {
    // Create and register - registry takes ownership directly
    registry.register_command({"step", "s"}, make_unique<StepCommand>());
    registry.register_command({"continue", "c"}, make_unique<ContinueCommand>());
    registry.register_command({"regs", "registers"}, make_unique<DisplayRegistersCommand>());
    registry.register_command({"break", "b"}, make_unique<AddBreakpointCommand>());
    registry.register_command({"delete", "d"}, make_unique<RemoveBreakpointCommand>());
    registry.register_command({"run", "r"}, make_unique<RunCommand>());
    registry.register_command({"list", "l"}, make_unique<ListBreakpointsCommand>());
    registry.register_command({"help", "h"}, make_unique<HelpCommand>());
    registry.register_command({"quit", "q"}, make_unique<QuitCommand>());
}
