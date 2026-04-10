#ifdef _INTERNALS_INCLUDES_

    /**
     * Support operations for yielding addressed operands
     *
     * These are static to help support register pinning in the interpreter
     */

    static Word readWord(Bus const& oOutside, Address const iAddress) noexcept {
        return oOutside.readByte(iAddress) | (oOutside.readByte(iAddress + 1) << 8);
    }


    /**
    * $NN
    */
    static Byte addrZeroPageByte(Bus const& oOutside, Address const iProgramCounter) noexcept {
        return oOutside.readByte( // unsigned 8-bit value loaded from...
            iProgramCounter + 1    // operand byte
        );
    }

    /**
    * $NNNN
    */
    static Word addrAbsoluteByte(Bus const& oOutside, Address const iProgramCounter) noexcept {
        return readWord(         // unsigned 16-bit value loaded from...
            oOutside,
            iProgramCounter + 1  // operand bytes
        );
    }

    /**
    * $NNNN,X
    */
    static Word addrAbsoluteXByte(Bus const& oOutside, Address const iProgramCounter, Byte const iXIndex) noexcept {
        return readWord(         // unsigned 16-bit value loaded from...
            oOutside,
            iProgramCounter + 1  // operand bytes, offset by...
        ) + iXIndex;             // unsigned 8-bit index in X register
    }

    /**
    * $NNNN,Y
    */
    static Word addrAbsoluteYByte(Bus const& oOutside, Address const iProgramCounter, Byte const iYIndex) noexcept {
        return readWord(         // unsigned 16-bit value loaded from...
            oOutside,
            iProgramCounter + 1  // operand bytes, offset by...
        ) + iYIndex;             // unsigned 8-bit index in Y register
    }

    /**
    * $NN,X (wraps in zero page)
    */
    static Byte addrZeroPageXByte(Bus const& oOutside, Address const iProgramCounter, Byte const iXIndex) noexcept {
        return (
            oOutside.readByte(       // unsigned 8-bit value loaded from...
                iProgramCounter + 1  // operand byte, offset by...
            ) + iXIndex              // unsigned 8-bit value in X register...
        );                // wrapped to zero page
    }

    /**
    * $NN,Y (wraps in zero page)
    */
    static Byte addrZeroPageYByte(Bus const& oOutside, Address const iProgramCounter, Byte const iYIndex) noexcept {
        return (
            oOutside.readByte(      // unsigned 8-bit value loaded from...
                iProgramCounter + 1 // operand byte, offset by...
            ) + iYIndex             // unsigned 8-bit value in Y register...
        );               // wrapped to zero page
    }

    /**
    * ($NN,X) (wraps in zero page)
    */
    static Word addrPreIndexZeroPageXByte(Bus const& oOutside, Address const iProgramCounter, Byte const iXIndex) noexcept {
        return readWord(                 // unsigned 16-bit value at address indicated by...
            oOutside,
            (
                oOutside.readByte(       // unsigned 8-bit value loaded from...
                    iProgramCounter + 1  // operand byte, offset by...
                ) + iXIndex              // unsigned 8-bit value in X register...
            ) & 0xFF                     // wrapped to zero page
        );
    }

    /**
    * ($NN),Y
    */
    static Word addrPostIndexZeroPageYByte(Bus const& oOutside, Address const iProgramCounter, Byte const iYIndex) noexcept {
        return readWord(              // unsigned 16-bit value at address indicated by...
            oOutside,
            oOutside.readByte(        // unsigned 8-bit value loaded from
                iProgramCounter + 1   // operand byte
            )                         // offset by...
        ) + iYIndex;                  //   unsigned 8-bit value in Y register
    }

#   define INTERNALS_ADDRESSING
#endif

