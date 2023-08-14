#include <complex>
#include <float.h>
#include <stdio.h>
#include <stdint.h>
#include <memory>
#include <unordered_map>
#include <map>
#include <functional>

typedef uint8_t Byte;
typedef uint16_t Word;
typedef uint32_t DWord;

struct Memory {

    Memory(DWord total_size_in_bytes) {
        size = total_size_in_bytes;
        data = new Byte[size];
    }

    ~Memory() {
        delete[] data;
    }

    void initialize() {
        for (DWord i = 0; i < size; ++i) {
            data[i] = 0;
        }
    }

    Byte read(Word address) {
        if (address > size) return 0;

        return data[address];
    }
    bool write(Word address, Byte value) { 
        if (address > size) return false;

        data[address] = value;
        return true;
    }
    private:

    Word size;
    Byte* data;
};
struct StatusFlags {
    bool C; // Carry Flag
    bool Z; // Zero Flag
    bool I; // Interrupt Disable
    bool D; // Decimal Mode
    bool B; // Break Command
    bool V; // Overflow Flag
    bool N; // Negative Flag
};
struct Cpu {
    enum OpCode {
        BRK = 0x00,
        ORA_X_IND = 0x01,
        ORA_ZP = 0x05,
        ASL_ZP = 0x06,
        PHP = 0x08,
        ORA_IMM = 0x09,
        ASL_A = 0x0A,
        ORA_ABS = 0x0D,
        ASL_ABS = 0x0E,
        BPL = 0x10,
        ORA_IND_Y = 0x11,
        ORA_ZP_X = 0x15,
        ASL_ZP_X = 0x16,
        CLC = 0x18,
        ORA_ABS_Y = 0x19,
        ORA_ABS_X = 0x1D,
        ASL_ABS_X = 0x1E,

        LDY_IMM = 0xA0,
        LDA_X_IND = 0xA1,
        LDX_IMM = 0xA2,
        LDA_ZP = 0xA5,
        LDA_IMM = 0xA9,
        LDA_ABS = 0xAD,
        LDA_IND_Y = 0xB1,
        LDA_ZP_X = 0xB5,
        LDA_ABS_Y = 0xB9,
        LDA_ABS_X = 0xBD,
    };

    Cpu(Memory program_memory) : mem(program_memory) {
        reset();
        init_opcode_map();
    }

    void init_opcode_map() {
        instruction_map[OpCode::LDA_IMM] = &Cpu::lda_immediate;
        instruction_map[OpCode::LDX_IMM] = &Cpu::ldx_immediate;
        instruction_map[OpCode::LDY_IMM] = &Cpu::ldy_immediate;
    }

    void reset() {
        PC = 0xFFFC;
        SP = 0x0100;
        A = 0;
        X = 0;
        Y = 0;
        flags = {0};
    }

    void tick() {
        //system("@cls||clear");
        //printf("tick\n");
        OpCode instruction = static_cast<OpCode>(next_byte());

        auto iter = instruction_map.find(instruction);
        if (iter == instruction_map.end()) {
            illegal();
            return;
        }

        iter->second(this);
    }

    private:

    Memory mem; // Memory

    std::unordered_map<OpCode, std::function<void(Cpu*)>> instruction_map;
    std::map<OpCode, void (Cpu::*)()> instruction_map;
    // Registers
    Word PC; // Program Counter
    Byte SP; // Stack Pointer

    Byte A; // Accumulator
    Byte X; // Index Register X
    Byte Y; // Index Register Y

    StatusFlags flags; // Processor Status Flags
             
    // Fetching
    Byte next_byte() {
        Byte result = mem.read(PC);
        ++PC;
        return result;
    }
    Word next_word_little_endian() {
        Word result = mem.read(PC);
        ++PC;
        result |= mem.read(PC) << 8;
        ++PC;
        return result;
    }
    Word next_word_big_endian() {
        Word result = mem.read(PC) << 8;
        ++PC;
        result |= mem.read(PC);
        ++PC;
        return result;
    }


    // Instructions
    void illegal() {
        std::printf("Illegal instruction at 0x%04X\n", PC - 1);
        //exit(1);
    }
    void not_implemented() {
        std::printf("Instruction not implemented at 0x%04X\n", PC - 1);
        exit(1);
    }
    void lda_immediate() {
        A = next_byte();
    }
    void ldx_immediate() {
        X = next_byte();
    }
    void ldy_immediate() {
        Y = next_byte();
    }
};
int main()
{
    Cpu cpu((Memory(0xFFFF)));

    while (true)
    {
        cpu.tick();
    }

    return 0;
}
