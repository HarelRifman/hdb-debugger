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

---

## How It Works

### Architecture Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                           main.cpp                              │
│                    (Entry point + REPL loop)                    │
└─────────────────────────────┬───────────────────────────────────┘
                              │ user input
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                      CommandRegistry                            │
│              (Maps command names → Command objects)             │
│                                                                 │
│   "break" ──┐      "step" ───┐      "continue" ──┐              │
│   "b" ──────┼─→ AddBreakpoint   "s" ─┼─→ Step    "c" ─┼─→ Continue     │
└─────────────┼───────────────────────┼────────────────┼──────────┘
              │                       │                │
              └───────────────────────┴────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                         Debugger                                │
│                  (State + Process Control)                      │
│                                                                 │
│   - child_pid        - wait_for_signal()                        │
│   - breakpoints      - add_breakpoint()                         │
│   - running          - remove_breakpoint()                      │
└─────────────────────────────┬───────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                        Breakpoint                               │
│                   (INT3 manipulation)                           │
│                                                                 │
│   - enable()   → writes 0xCC to memory                          │
│   - disable()  → restores original byte                         │
│   - handle_hit() → restore, step, re-enable                     │
└─────────────────────────────────────────────────────────────────┘
```

### Startup Flow

```
┌──────────────┐         ┌──────────────┐
│    Parent    │         │    Child     │
│  (Debugger)  │         │  (Target)    │
└──────┬───────┘         └──────┬───────┘
       │                        │
       │◄─────── fork() ───────►│
       │                        │
       │                        │ ptrace(TRACEME)
       │                        │ execvp(program)
       │                        │ ──► STOPPED
       │                        │
       │ waitpid()              │
       │ ◄──── SIGTRAP ─────────│
       │                        │
       │ "Debugger attached"    │
       │                        │
       ▼                        ▼
   REPL loop              waiting...
```

### Command Flow (example: "break")

```
User types: "break 0x401176"
              │
              ▼
┌─────────────────────────────┐
│  main.cpp: split input      │
│  cmd="break", args=["0x..."]│
└─────────────┬───────────────┘
              │
              ▼
┌─────────────────────────────┐
│  CommandRegistry::execute() │
│  finds AddBreakpointCommand │
└─────────────┬───────────────┘
              │
              ▼
┌─────────────────────────────┐
│  AddBreakpointCommand       │
│  ::execute(debugger, args)  │
└─────────────┬───────────────┘
              │
              ▼
┌─────────────────────────────┐
│  Debugger::add_breakpoint() │
│  creates Breakpoint object  │
└─────────────┬───────────────┘
              │
              ▼
┌─────────────────────────────┐
│  Breakpoint::enable()       │
│  PTRACE_PEEKDATA (save)     │
│  PTRACE_POKEDATA (0xCC)     │
└─────────────────────────────┘
```

### Breakpoint Hit Flow

```
Child executes instruction at 0x401176
              │
              ▼
┌─────────────────────────────┐
│  CPU hits 0xCC (INT3)       │
│  Generates SIGTRAP          │
│  RIP = 0x401177 (after INT3)│
└─────────────┬───────────────┘
              │
              ▼
┌─────────────────────────────┐
│  Debugger::wait_for_signal()│
│  waitpid() returns          │
│  WSTOPSIG == SIGTRAP        │
└─────────────┬───────────────┘
              │
              ▼
┌─────────────────────────────┐
│  Check: RIP-1 == breakpoint?│
│  Yes → Breakpoint::handle() │
└─────────────┬───────────────┘
              │
              ▼
┌─────────────────────────────┐
│  handle_hit():              │
│  1. disable() - restore byte│
│  2. RIP -= 1  - rewind      │
│  3. single step             │
│  4. enable() - set 0xCC     │
└─────────────────────────────┘
```

---

## Project Structure

```
src/
├── main.cpp                     # Entry point, fork, REPL loop
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

## Design Patterns Used

- **Command Pattern** - Each debugger command is a class implementing `Command` interface
- **Factory Pattern** - `CommandFactory` creates and registers all commands
- **Registry Pattern** - `CommandRegistry` maps command names to handlers
