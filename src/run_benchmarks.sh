#!/bin/bash

# Configuration
CC="g++"
BASE_CFLAGS="-Wall -Wextra -march=native -falign-labels=16 -Ofast --std=c++20 -I./include -fno-lto"
BENCH_SECONDS=30

# List of interpreters (matching Makefile targets)
declare -A configs=(
    ["Runtime"]=" -DINTERPRET_SWITCH -DBUS_UNPINNED"
    ["StaticSC"]=" -DSTATIC_SYSTEM -DINTERPRET_SWITCH -DBUS_UNPINNED"
    ["StaticSCPin"]=" -DSTATIC_SYSTEM -DINTERPRET_SWITCH"
    ["StaticMaxGoto"]=" -DSTATIC_SYSTEM"
    ["StaticMaxGotoLTO"]=" -DSTATIC_SYSTEM -flto"
)

# Benchmark metadata: bin_file instr_per_op
benchmarks=(
    "alu_bench.bin 2306"
    "memory_bench.bin 1025"
    "call_bench.bin 1281"
    "branch_bench.bin 1537"
    "mixed_bench.bin 2050"
)

# Prepare result storage
results_file=$(mktemp)

# List of interpreters (matching Makefile targets)
declare -A configs=(
    ["Runtime"]=" -DBUS_UNPINNED"
    ["StaticSC"]=" -DSTATIC_SYSTEM -DINTERPRET_SWITCH -DBUS_UNPINNED"
    ["StaticSCPin"]=" -DSTATIC_SYSTEM -DINTERPRET_SWITCH"
    ["StaticMaxGoto"]=" -DSTATIC_SYSTEM"
    ["StaticMaxGotoLTO"]=" -DSTATIC_SYSTEM -flto"
)

# Run Benchmarks
for name in "StaticSC" "StaticSCPin" "StaticMaxGoto" "StaticMaxGotoLTO"; do
    flags=${configs[$name]}
    echo "=========================================================="
    echo " Compiling and Running Interpreter: $name"
    echo " Flags: $flags"
    echo "=========================================================="

    # Compile the harness for this specific configuration
    LDFLAGS=""
    if [[ $flags == *"flto"* ]]; then
        LDFLAGS="-flto"
    fi

    $CC $BASE_CFLAGS $flags bench_harness.cpp memory.cpp -o "bench_$name" $LDFLAGS

    if [ $? -ne 0 ]; then
        echo "Compilation of $name failed."
        continue
    fi

    # Run each benchmark binary
    for b in "${benchmarks[@]}"; do
        set -- $b
        bin=$1
        instrs=$2

        if [ -f "$bin" ]; then
            echo "Running $bin..."
            output=$(timeout $((BENCH_SECONDS + 5))s "./bench_$name" "$bin" "$instrs" "$BENCH_SECONDS")
            mips=$(echo "$output" | grep -oP '\[\K[0-9.]+(?= MIPS\])')
            echo "$name,$bin,${mips:-0.0}" >> "$results_file"
        fi
    done
done

# Print final table
# ... (rest of the script remains same)


# Print final table
echo ""
echo "================================================================================"
echo "                           6502 EMULATOR BENCHMARK RESULTS"
echo "================================================================================"
printf "%-18s | %-12s | %-12s | %-12s | %-12s | %-12s\n" "Interpreter" "ALU" "Memory" "Call" "Branch" "Mixed"
echo "-------------------|--------------|--------------|--------------|--------------|--------------"

for name in "StaticSC" "StaticSCPin" "StaticMaxGoto" "StaticMaxGotoLTO"; do
    alu=$(grep "^$name,alu_bench.bin," "$results_file" | cut -d',' -f3)
    alu=${alu:-0.0}
    mem=$(grep "^$name,memory_bench.bin," "$results_file" | cut -d',' -f3)
    mem=${mem:-0.0}
    cal=$(grep "^$name,call_bench.bin," "$results_file" | cut -d',' -f3)
    cal=${cal:-0.0}
    bra=$(grep "^$name,branch_bench.bin," "$results_file" | cut -d',' -f3)
    bra=${bra:-0.0}
    mix=$(grep "^$name,mixed_bench.bin," "$results_file" | cut -d',' -f3)
    mix=${mix:-0.0}
    
    printf "%-18s | %12.2f | %12.2f | %12.2f | %12.2f | %12.2f\n" "$name" "$alu" "$mem" "$cal" "$bra" "$mix"
done
echo "================================================================================"

rm "$results_file"
rm -f bench_Runtime bench_StaticSC bench_StaticSCPin bench_StaticMaxGoto bench_StaticMaxGotoLTO
