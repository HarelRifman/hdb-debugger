# Compiler and flags
CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -g
CFLAGS = -no-pie -g

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

# Default target - build debugger and test program
all: $(TARGET) $(TEST_PROG)

# Build the debugger
$(TARGET): $(ALL_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -o $@ $(ALL_SRC)

# Build test program
$(TEST_PROG): test/test_program.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*

# Show test program functions (for breakpoints)
funcs: $(TEST_PROG)
	@echo "Functions in test_program:"
	@nm $(TEST_PROG) | grep -E "^[0-9a-f]+ T " | grep -v "^.* T _"

# Help
help:
	@echo "Makefile targets:"
	@echo "  all   - Build debugger and test program (default)"
	@echo "  clean - Remove build artifacts"
	@echo "  funcs - Show functions in test program (for breakpoints)"
	@echo ""
	@echo "Usage:"
	@echo "  make"
	@echo "  ./build/HDB ./build/test_program"
	@echo ""
	@echo "See test.md for testing instructions"

.PHONY: all clean funcs help
