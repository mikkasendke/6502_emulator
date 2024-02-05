#pragma once

#include "memory.h"
#include "opcodes.h"
#include "types.h"

class Cpu {
public:
    explicit Cpu(const Memory& memory);
    void reset();
    void tick();

private:
    Memory mem;

    // Registers
    Word PC = 0; // Program counter
    Byte SP = 0; // Stack pointer
    Byte A = 0;
    Byte X = 0;
    Byte Y = 0;

    struct {
        bool C : 1; // Carry Flag
        bool Z : 1; // Zero Flag
        bool I : 1; // Interrupt Disable
        bool D : 1; // Decimal
        bool B : 1; // Break (probably no use)
        bool V : 1; // Overflow (probably no use)
        bool N : 1; // Negative
    } StatusFlags = {};

    [[nodiscard]] std::optional<OpCode> next_instruction();
    [[nodiscard]] bool execute_instruction(const OpCode& opcode);

    [[nodiscard]] bool loadIntoRegister(Byte &reg, const Byte &address);

    [[nodiscard]] bool storeIntoMemory(const Byte &value, const Byte &address);


    /* Each instruction will have a function
     * that will execute the instruction
     * using helpers etc. and then update
     * the program counter
     */

    bool LDA_IMM(const Byte &address);

    bool LDX_IMM(const Byte &address);

    bool LDY_IMM(const Byte &address);
};
