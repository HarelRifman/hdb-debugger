# HDB Architecture

Deep dive into how the debugger works internally.

## Architecture Overview

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
│    "break" ─┬─→ AddBreakpoint    "continue" ─┬─→ Continue       │
│    "b" ─────┘                    "c" ────────┘                  │
│    "step" ──┬─→ Step             "regs" ────────→ DisplayRegs   │
│    "s" ─────┘                                                   │
└─────────────────────────────┬───────────────────────────────────┘
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

## Startup Flow

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

## Command Flow

Example: `break 0x401176`

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

## Breakpoint Hit Flow

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

