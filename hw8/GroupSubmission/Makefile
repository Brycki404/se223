# ============================================================
#   Multi‑Language Pipe Ring — Makefile
#   Builds:
#       c_process   (C)
#       cpp_process (C++)
#       py_process  (Python syntax check only)
# ============================================================

ifeq ($(OS),Windows_NT)
    DETECTED_OS := windows
else
    DETECTED_OS := linux
endif

CC      = gcc
CXX     = g++
CFLAGS  = -Wall -Wextra -O2
CXXFLAGS= -Wall -Wextra -O2

BUILD_DIR = build


ifeq ($(OS),Windows_NT)
    EXE := .exe
    RUN_PREFIX :=
    GREEN = $(shell printf "\033[32m")
	YELLOW = $(shell printf "\033[33m")
	BLUE = $(shell printf "\033[34m")
	RESET = $(shell printf "\033[0m")
else
    EXE :=
    RUN_PREFIX := ./
    GREEN   = \033[32m
    YELLOW  = \033[33m
    BLUE    = \033[34m
    RESET   = \033[0m
endif

# Targets to build everything
all: prep $(BUILD_DIR)/main$(EXE) $(BUILD_DIR)/process$(EXE) pycheck
	@echo "$(GREEN)Build complete!$(RESET)"

# Create build directory if missing
prep:
	@mkdir -p $(BUILD_DIR)

# Compile C controller
$(BUILD_DIR)/main$(EXE): main.c
	@echo "$(BLUE)Compiling C process...$(RESET)"
	$(CC) $(CFLAGS) $< -o $@
	
# Compile C++ stage
$(BUILD_DIR)/process$(EXE): process.cpp
	@echo "$(BLUE)Compiling C++ process...$(RESET)"
	$(CXX) $(CXXFLAGS) $< -o $@

# Python syntax check (no compile)
pycheck: process.py
	@echo "$(BLUE)Checking Python syntax...$(RESET)"
	@python3 -m py_compile process.py || (echo "$(YELLOW)Python syntax error!$(RESET)" && exit 1)

# Run the system with default arguments
run: all
	@echo "$(GREEN)Running system (default: on, max_passes=10)...$(RESET)"
	@$(RUN_PREFIX)$(BUILD_DIR)/main$(EXE) on 10

# Clean build artifacts
clean:
	@echo "$(YELLOW)Cleaning build directory...$(RESET)"
	@rm -rf $(BUILD_DIR)

.PHONY: all prep pycheck run clean