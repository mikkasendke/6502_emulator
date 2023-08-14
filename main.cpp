#include <float.h>
#include <stdio.h>
#include <stdint.h>
#include <memory>

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

    Byte read(Word address) {
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

    enum OpCodes {
        BRK = 0x00,
        ORA_IX = 0x01,
        ORA_ZP = 0x05,
        ASL_ZP = 0x06,
        LDA_I = 0xA9,
        LDX_I = 0xA2,
        LDY_I = 0xA0,
    };

    Cpu(Memory program_memory) : mem(program_memory) {
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
        Byte instruction = next_byte();

        switch (instruction) {
            case LDA_I: {
                lda_immediate();
                break;
            }
            case LDX_I: {
                ldx_immediate();
                break;
            }
            case LDY_I: {
                ldy_immediate();
                break;
            }
            default: {
                printf("Unknown instruction: %02X\n", instruction);
                break;
            }
        }
    }
    private:

    Memory mem; // Memory

    // Registers
    Word PC; // Program Counter
    Byte SP; // Stack Pointer

    Byte A; // Accumulator
    Byte X; // Index Register X
    Byte Y; // Index Register Y

    StatusFlags flags; // Processor Status Flags
                
    Byte next_byte() {
        Byte result = mem.read(PC);
        ++PC;
        return result;
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
    cpu.reset();


    return 0;
}
