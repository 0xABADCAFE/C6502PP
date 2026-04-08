# --- Configuration ---
CC      := g++
BASE_CFLAGS := -Wall -Wextra -march=native -falign-labels=16 -Ofast --std=c++20 -I./include -fconcepts-diagnostics-depth=2 -fcf-protection=none

SRCS    := main.cpp memory.cpp

# --- The "Evolution of Performance" Suite ---

.PHONY: bench clean build_run runtime_test compiletime_test_sc compiletime_test_sc_pin compiletime_max

# One-shot command to run all benchmarks and keep the binaries
bench:
	@echo "=========================================================="
	@echo " STARTING THE 6502 PERFORMANCE JOURNEY"
	@echo "=========================================================="
	@$(MAKE) runtime_test
	@$(MAKE) compiletime_test_sc
	@$(MAKE) compiletime_test_sc_pin
	@$(MAKE) compiletime_max
	@echo "=========================================================="
	@echo " BENCHMARK COMPLETE - Binaries saved as test_*"
	@echo "=========================================================="

# 1. THE TRAGEDY (Runtime System, Switch/Case, Unpinned, No LTO)
runtime_test:
	@echo "\n--- 1. RUNTIME SYSTEM (Virtual, Switch, Unpinned, No LTO) ---"
	@$(MAKE) build_run BIN_NAME="test_runtime" FLAGS="-DINTERPRET_SWITCH -DBUS_UNPINNED -fno-lto"

# 2. THE TRANSITION (Compile Time System, Switch/Case, Unpinned, No LTO)
compiletime_test_sc:
	@echo "\n--- 2. COMPILE TIME (Static, Switch, Unpinned, No LTO) ---"
	@$(MAKE) build_run BIN_NAME="test_sc" FLAGS="-DSTATIC_SYSTEM -DINTERPRET_SWITCH -DBUS_UNPINNED -fno-lto"

# 3. THE TUNED (Compile Time System, Switch/Case, Pinned, No LTO)
compiletime_test_sc_pin:
	@echo "\n--- 3. COMPILE TIME (Static, Switch, Pinned, No LTO) ---"
	@$(MAKE) build_run BIN_NAME="test_sc_pin" FLAGS="-DSTATIC_SYSTEM -DINTERPRET_SWITCH -fno-lto"

# 4. THE BEAST (Static System, Threaded Dispatch, Pinned, Full LTO)
compiletime_max:
	@echo "\n--- 4. COMPILE TIME (Static, Threaded, Pinned, LTO) ---"
	@$(MAKE) build_run BIN_NAME="test_max" FLAGS="-DSTATIC_SYSTEM -flto" LDFLAGS="-flto"

# --- Internal Helper Rule ---

# Compiles directly to the unique binary name and runs it
build_run:
	rm -f *.o
	$(CC) $(BASE_CFLAGS) $(FLAGS) $(SRCS) -o $(BIN_NAME) $(LDFLAGS)
	./$(BIN_NAME)

clean:
	rm -f *.o test test_runtime test_sc test_sc_pin test_max
