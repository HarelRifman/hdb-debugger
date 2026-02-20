# HDB - Harel's Debugger

A simple x86-64 Linux debugger built with `ptrace`.

## Features

- Set/remove breakpoints at memory addresses
- Single-step execution
- View CPU registers
- Continue execution

## Build

```bash
make
```

## Usage

```bash
./build/HDB ./build/test_program
```

```
hdb> break 0x401176
hdb> run
hdb> regs
hdb> continue
hdb> quit
```

## Commands

| Command | Alias | Description |
|---------|-------|-------------|
| `break <addr>` | `b` | Set breakpoint |
| `delete <addr>` | `d` | Remove breakpoint |
| `list` | `l` | List breakpoints |
| `run` | `r` | Start program |
| `continue` | `c` | Continue execution |
| `step` | `s` | Single step |
| `regs` | | Show registers |
| `help` | `h` | Show help |
| `quit` | `q` | Exit |

## Project Structure

```
src/
├── main.cpp                     # Entry point, fork, main loop
├── core/
│   ├── debugger.cpp/h           # Debugger state and ptrace calls
│   └── breakpoint.cpp/h         # INT3 breakpoint logic
├── registry/
│   ├── command_registry.cpp/h   # Maps names → commands
│   └── command_factory.cpp/h    # Creates all command objects
└── commands/
    ├── command.h                # Abstract Command interface
    ├── run_command.cpp/h        # Start program execution
    ├── continue_command.cpp/h   # Continue after breakpoint
    ├── step_command.cpp/h       # Single instruction step
    ├── add_breakpoint_command.cpp/h
    ├── remove_breakpoint_command.cpp/h
    ├── list_breakpoints_command.cpp/h
    ├── display_registers_command.cpp/h
    ├── help_command.cpp/h
    └── quit_command.cpp/h
```

## Design Patterns

- **Command Pattern** - Each debugger command is a class implementing `Command` interface
- **Factory Pattern** - `CommandFactory` creates and registers all commands
- **Registry Pattern** - `CommandRegistry` maps command names to handlers

---

For implementation details, see [ARCHITECTURE.md](ARCHITECTURE.md).
If you want to test it for yourself and see how it works check out [TEST.md](TEST.md)