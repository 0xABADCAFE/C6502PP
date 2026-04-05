#ifndef C6502PP_HPP
#	define C6502PP_HPP
#	include <cstdint>
#	include "concepts.hpp"
#	include "constants.hpp"

namespace C6502PP {

    /**
     * CPU
     */
    template <BusDevice Bus>
    struct CPU {

        static_assert(0 == (STACK_BASE & 0xFF), "STACK_BASE is not page aligned");
        static_assert((STACK_TOP > STACK_BASE), "STACK_TOP must be higher than STACK_BASE");

        Bus &oOutside;

        Address iProgramCounter = {0};
        Byte iAccumulator = {0};
        Byte iStatus = {0};
        Byte iXIndex = {0};
        Byte iYIndex = {0};
        Byte iStackPointer = {0};

        CPU(Bus& oBus): oOutside(oBus) { reset(); }

        void reset() noexcept {
            iAccumulator    = 0;
            iXIndex         = 0;
            iYIndex         = 0;
            iStackPointer   = STACK_TOP - STACK_BASE; // Offset in the page at STACK_BASE
            iProgramCounter = readWord(VEC_RES);      // Load from reset vector
            iStatus         = F_ZERO;
        }

        CPU& softReset() noexcept {
            reset();
            return *this;
        }

        CPU& hardReset() noexcept {
            reset();
            return *this;
        }

        Word readWord(Address iAddress) noexcept {
            return oOutside.readByte(iAddress) | (oOutside.readByte(iAddress + 1) << 8);
        }

        void setNZ(Byte iValue) noexcept {
            iStatus &= F_CLR_NZ;
            iStatus |= iValue ? (iValue & F_NEGATIVE) : F_ZERO;
        }

        bool assertNZ(Byte iNResult, Byte iZResult) noexcept {
            if (iStatus & F_ZERO) {
                if (iZResult) {
                    std::printf(
                        "Z mismatch SR:true Z:0x%02X\n",
                        (unsigned)iZResult
                    );
                    return false;
                }
            } else {
                if (!iZResult) {
                    std::printf(
                        "Z mismatch SR:false Z:0x%02X\n",
                        (unsigned)iZResult
                    );
                    return false;
                }
            }

            if (iStatus & F_NEGATIVE) {
                if (!(iNResult & F_NEGATIVE)) {
                    std::printf(
                        "N mismatch SR:true N:0x%02X\n",
                        (unsigned)iNResult
                    );
                    return false;
                }
            } else {
                if (iNResult & F_NEGATIVE) {
                    std::printf(
                        "N mismatch SR:false NZ:0x%02X\n",
                        (unsigned)iNResult
                    );
                    return false;
                }
            }
            return true;
        }

#   define _INTERNALS_INCLUDES_
#   include "internals/addressing.hpp"
#   include "internals/alu.hpp"
#   include "internals/run.hpp"
#   undef _INTERNALS_INCLUDES_

    };


}

#endif
