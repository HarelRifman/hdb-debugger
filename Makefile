# Compiler and flags
CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -g
CFLAGS = -no-pie -g
PIE_CFLAGS = -pie -fPIE -g

# Directories
SRC_DIR = src
BUILD_DIR = build
CORE_DIR = $(SRC_DIR)/core
CMD_DIR = $(SRC_DIR)/commands
REG_DIR = $(SRC_DIR)/registry

# Source files
CORE_SRC = $(wildcard $(CORE_DIR)/*.cpp)
CMD_SRC = $(wildcard $(CMD_DIR)/*.cpp)
REG_SRC = $(wildcard $(REG_DIR)/*.cpp)
MAIN_SRC = $(SRC_DIR)/main.cpp

ALL_SRC = $(MAIN_SRC) $(CORE_SRC) $(CMD_SRC) $(REG_SRC)

# Targets
TARGET = $(BUILD_DIR)/HDB
TEST_PROG = $(BUILD_DIR)/test_program
TEST_PROG_PIE = $(BUILD_DIR)/test_program_pie

# Default target - build debugger and both test program variants
all: $(TARGET) $(TEST_PROG) $(TEST_PROG_PIE)

# Build the debugger
$(TARGET): $(ALL_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -o $@ $(ALL_SRC)

# Build test program - non-PIE (ET_EXEC), fixed link-time addresses
$(TEST_PROG): test/test_program.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Build test program - PIE (ET_DYN), same source, randomized load address
$(TEST_PROG_PIE): test/test_program.c | $(BUILD_DIR)
	$(CC) $(PIE_CFLAGS) -o $@ $<

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*

# Show test program functions (for breakpoints). Static addresses are the
# same for the PIE build too -- nm reads link-time (vaddr) addresses, not
# wherever the kernel happens to load the binary at.
funcs: $(TEST_PROG)
	@echo "Functions in test_program:"
	@nm $(TEST_PROG) | grep -E "^[0-9a-f]+ T " | grep -v "^.* T _"

# Help
help:
	@echo "Makefile targets:"
	@echo "  all   - Build debugger and test program (non-PIE + PIE) (default)"
	@echo "  clean - Remove build artifacts"
	@echo "  funcs - Show functions in test program (for breakpoints)"
	@echo ""
	@echo "Usage:"
	@echo "  make"
	@echo "  ./build/HDB ./build/test_program       # non-PIE"
	@echo "  ./build/HDB ./build/test_program_pie   # PIE - same break <addr> commands"
	@echo ""
	@echo "See test.md for testing instructions"

.PHONY: all clean funcs help
