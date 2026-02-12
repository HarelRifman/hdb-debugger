# Testing the HDB Debugger

## Build

```bash
make
```

This builds:
- `build/HDB` - the debugger
- `build/test_program` - a sample program to debug

## Find Function Address

```bash
make funcs
```

Example output:
```
0000000000401176 T function_to_break
0000000000401196 T main
```

## Run the Debugger

```bash
./build/HDB ./build/test_program
```

## Test Commands

### 1. Set a breakpoint and run

```
hdb> break 0x401176
hdb> run
```

Expected: Program runs and stops at the breakpoint.

### 2. Continue execution

```
hdb> continue
```

Expected: Program continues until next breakpoint hit or exits.

### 3. View registers

```
hdb> break 0x401176
hdb> run
hdb> regs
```

Expected: Shows CPU registers (rax, rbx, rip, etc.)

### 4. Single step

```
hdb> break 0x401176
hdb> run
hdb> step
hdb> regs
```

Expected: Executes one instruction, then shows updated rip.

### 5. List breakpoints

```
hdb> break 0x401176
hdb> list
```

Expected: Shows all breakpoints and their status.

### 6. Remove breakpoint

```
hdb> break 0x401176
hdb> delete 0x401176
hdb> list
```

Expected: Breakpoint is removed.

### 7. Quit

```
hdb> quit
```

Expected: Debugger exits and terminates the child process.

## Available Commands

| Command | Alias | Description |
|---------|-------|-------------|
| `break <addr>` | `b` | Set breakpoint at address |
| `delete <addr>` | `d` | Remove breakpoint |
| `list` | `l` | List all breakpoints |
| `run` | `r` | Start the program |
| `continue` | `c` | Continue execution |
| `step` | `s` | Single step one instruction |
| `regs` | | Display registers |
| `help` | `h` | Show help |
| `quit` | `q` | Exit debugger |

## Notes

- Addresses must be in hex format with `0x` prefix
- The test program has a loop that calls `function_to_break` 3 times
- Set a breakpoint on `function_to_break` to see it hit multiple times

