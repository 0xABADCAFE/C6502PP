# --- Configuration ---
CC      := g++
BASE_CFLAGS := -Wall -Wextra -march=native -falign-labels=16 -Ofast --std=c++20 -I./include -fconcepts-diagnostics-depth=2 -fcf-protection=none

SRCS    := main.cpp memory.cpp
BENCH_SRCS := bench_harness.cpp memory.cpp

include benchmark_matrix.mk

# --- The "Evolution of Performance" Suite ---

.PHONY: bench clean build_run build_bench runtime_test runtime_test_lto compiletime_test_sc compiletime_test_sc_lto compiletime_test_sc_pin compiletime_test_sc_pin_lto compiletime_max compiletime_max_lto print_benchmark_configs

# One-shot command to run all benchmarks and keep the binaries
bench:
	@echo "=========================================================="
	@echo " Building"
	@echo "=========================================================="
	@$(MAKE) runtime_test
	@$(MAKE) runtime_test_lto
	@$(MAKE) compiletime_test_sc
	@$(MAKE) compiletime_test_sc_lto
	@$(MAKE) compiletime_test_sc_pin
	@$(MAKE) compiletime_test_sc_pin_lto
	@$(MAKE) compiletime_max
	@$(MAKE) compiletime_max_lto
	@echo "=========================================================="
	@echo " Completed - Binaries saved as test_*"
	@echo "=========================================================="

# 1. Runtime Abstracted, switch-case, no other frills
runtime_test:
	@echo "\n--- 1. Runtime abstracted, switch/case ---"
	@$(MAKE) build_run BIN_NAME="test_runtime" FLAGS="$(BENCH_FLAGS_Runtime)"

# 2. Runtime Abstracted, switch-case, LTO
runtime_test_lto:
	@echo "\n--- 2. Runtime abstracted, switch/case, full LTO ---"
	@$(MAKE) build_run BIN_NAME="test_runtime_lto" FLAGS="$(BENCH_FLAGS_RuntimeLTO)"


# 3. Compile Time Abstracted, switch-case, no other frills
compiletime_test_sc:
	@echo "\n--- 3. Compile-time abstracted, switch/case ---"
	@$(MAKE) build_run BIN_NAME="test_sc" FLAGS="$(BENCH_FLAGS_StaticSC)"

# 4. Compile Time Abstracted, switch-case, full LTO
compiletime_test_sc_lto:
	@echo "\n--- 4. Compile-time abstracted, switch/case, full LTO ---"
	@$(MAKE) build_run BIN_NAME="test_sc_lto" FLAGS="$(BENCH_FLAGS_StaticSCLTO)"

# 5. Compile Time Abstracted, switch-case, dependency ref pinning
compiletime_test_sc_pin:
	@echo "\n--- 5. Compile-time abstracted, switch/case, pinned ---"
	@$(MAKE) build_run BIN_NAME="test_sc_pin" FLAGS="$(BENCH_FLAGS_StaticSCPin)"

# 6. Compile Time Abstracted, switch-case, dependency ref pinning with LTO
compiletime_test_sc_pin_lto:
	@echo "\n--- 6. Compile-time abstracted, switch/case, pinned, full LTO ---"
	@$(MAKE) build_run BIN_NAME="test_sc_pin_lto" FLAGS="$(BENCH_FLAGS_StaticSCPinLTO)"

# 7. Compile Time Abstracted, goto and extras
compiletime_max:
	@echo "\n--- 7. Compile-time abstracted, computed goto, pinned ---"
	@$(MAKE) build_run BIN_NAME="test_max" FLAGS="$(BENCH_FLAGS_StaticMaxGoto)" LDFLAGS="$(BENCH_LDFLAGS_StaticMaxGoto)"

# 8. Compile Time Abstracted, goto and extras
compiletime_max_lto:
	@echo "\n--- 8. Compile-time abstracted, computed goto, pinned, full LTO ---"
	@$(MAKE) build_run BIN_NAME="test_max_lto" FLAGS="$(BENCH_FLAGS_StaticMaxGotoLTO)" LDFLAGS="$(BENCH_LDFLAGS_StaticMaxGotoLTO)"
# --- Internal Helper Rule ---

# Compiles directly to the unique binary name and runs it
build_run:
	rm -f *.o
	$(CC) $(BASE_CFLAGS) $(FLAGS) $(SRCS) -o $(BIN_NAME) $(LDFLAGS)
#	./$(BIN_NAME)

build_bench:
	rm -f *.o
	$(CC) $(BASE_CFLAGS) $(FLAGS) $(BENCH_SRCS) -o $(BIN_NAME) $(LDFLAGS)

print_benchmark_configs:
	@$(foreach name,$(BENCHMARK_TARGETS),printf '%s|%s|%s\n' '$(name)' '$(BENCH_FLAGS_$(name))' '$(BENCH_LDFLAGS_$(name))';)

clean:
	rm -f *.o test_* bench_Runtime bench_RuntimeLTO bench_StaticSC bench_StaticSCPin bench_StaticMaxGoto bench_harness
