#pragma once

#include "memory.h"
#include "types.h"

class Cpu {
    public:
        explicit Cpu(const Memory& memory);
        void reset();
        void tick();

    private:
        Memory mem;

        // Registers
        Word PC; // Program counter
        Byte SP; // Stack pointer
        Byte A;
        Byte X;
        Byte Y;

        struct StatusFlags {
            bool C : 1; // Carry Flag
            bool Z : 1; // Zero Flag
            bool I : 1; // Interrupt Disable
            bool D : 1; // Decimal
            bool B : 1; // Break (probably no use)
            bool V : 1; // Overflow (probably no use)
            bool N : 1; // Negative 
        };
};
