# Wrapper for building Forge. Requires CMake 3.20+ and a C++17 compiler.
# Usage: make build | run | clean

BUILD_DIR = build
BIN_DIR   = $(BUILD_DIR)/bin
FORGE     = $(BIN_DIR)/forge

.PHONY: build run clean help

build:
	@mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && cmake .. && cmake --build .
	@echo "Binary: $(FORGE)"

run: build
	@$(FORGE) help

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "Targets: build, run, clean"
