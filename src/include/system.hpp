#ifndef C6502PP_SYSTEM_HPP
#   define C6502PP_SYSTEM_HPP
#	include "concepts.hpp"
#   include "cpu.hpp"
#   include "bus/memory.hpp"

namespace C6502PP {

/**
 * CompileTymeSystem<Processor. BusDevice>
 *
 * Wraps an instance of a given Processor concept and BusDevice concept such that code generator is
 * able to inine away the read/write calls to the bus implementation.
 */
template <template <typename> typename CPUType, BusDevice MemoryBus>
struct alignas(NativeCacheLine) CompileTimeSystem {

    using CPU = CPUType<MemoryBus>;

    alignas(NativeCacheLine) CPU oCPU;
    alignas(NativeCacheLine) MemoryBus oBus;

    CompileTimeSystem() : oCPU(oBus) {
        std::printf("sizeof(CompileTimeSystem) = %zu bytes\n", sizeof(CompileTimeSystem));
    }

    CompileTimeSystem& run() {
        oCPU.run();
        return *this;
    }

    CompileTimeSystem& runFrom(Address iStart) {
        oCPU.setProgramCounter(iStart).run();
        return *this;
    }

    CompileTimeSystem& softReset() noexcept {
        oCPU.softReset();
        oBus.softReset();
        return *this;
    }

    CompileTimeSystem& hardReset() noexcept {
        oCPU.hardReset();
        oBus.hardReset();
        return *this;
    }

    void showStatus() const noexcept {
        oCPU.showStatus();
    }
};

};

#endif

