#ifndef C6502PP_BUS_MEMORY_HPP
#	define C6502PP_BUS_MEMORY_HPP
#	include "concepts.hpp"
#	include "constants.hpp"
#	include <cstring>
#   include <cstdio>

namespace C6502PP::Bus {

/**
 * Simple flat memory implementation of ByteAccessible concept
 */
struct SimpleMemory {
    Byte bytes[MEM_SIZE] = { 0 };

    Byte readByte(uint16_t address) const noexcept {
        return bytes[address];
    }

    void writeByte(Address address, Byte value) noexcept {
        bytes[address] = value;
    }

    SimpleMemory& softReset() noexcept { return *this; }
    SimpleMemory& hardReset() noexcept {
        std::memset(bytes, 0, sizeof(bytes));
        return *this;
    }

    /**
     * Load an image into memory at the given address. Attempts to load at most MEM_SIZE data.
     * Returns total bytes read.
     */
    size_t loadImage(char const* sImage, Address iLocation = 0) {
        size_t iResult = 0;
        std::FILE* pImage = std::fopen(sImage, "rb");
        if (pImage) {
            iResult = std::fread(bytes, 1, (MEM_SIZE - iLocation), pImage);
            std::fclose(pImage);
        }
        return iResult;
    }
};

}

#endif
