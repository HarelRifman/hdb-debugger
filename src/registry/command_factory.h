#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include "command_registry.h"

class CommandFactory {
public:
    // Creates all commands and registers them (registry takes ownership)
    static void create_and_register(CommandRegistry& registry);
};

#endif
