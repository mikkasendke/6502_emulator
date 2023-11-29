#include <set>
#include <cstdio>
#include <cstdint>
#include <memory>
#include <thread>
#include <map>

typedef uint8_t Byte;
typedef uint16_t Word;
typedef uint32_t DWord;

struct Memory {

    explicit Memory(DWord total_size_in_bytes) {
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
    enum class OpCode {
        BRK_IMP = 0x00,
        ORA_X_IND = 0x01,
        ORA_ZP = 0x05,
        ASL_ZP = 0x06,
        PHP_IMP = 0x08,
        ORA_IMM = 0x09,
        ASL_ACC = 0x0A,
        ORA_ABS = 0x0D,
        ASL_ABS = 0x0E,
        BPL_REL = 0x10,
        ORA_IND_Y = 0x11,
        ORA_ZP_X = 0x15,
        ASL_ZP_X = 0x16,
        CLC_IMP = 0x18,
        ORA_ABS_Y = 0x19,
        ORA_ABS_X = 0x1D,
        ASL_ABS_X = 0x1E,
        JSR_ABS = 0x20,
        AND_X_IND = 0x21,
        BIT_ZP = 0x24,
        AND_ZP = 0x25,
        ROL_ZP = 0x26,
        PLP_IMP = 0x28,
        AND_IMM = 0x29,
        ROL_ACC = 0x2A,
        BIT_ABS = 0x2C,
        AND_ABS = 0x2D,
        ROL_ABS = 0x2E,
        BMI_REL = 0x30,
        AND_IND_Y = 0x31,
        AND_ZP_X = 0x35,
        ROL_ZP_X = 0x36,
        SEC_IMP = 0x38,
        AND_ABS_Y = 0x39,
        AND_ABS_X = 0x3D,
        ROL_ABS_X = 0x3E,
        RTI_IMP = 0x40,
        EOR_X_IND = 0x41,
        EOR_ZP = 0x45,
        LSR_ZP = 0x46,
        PHA_IMP = 0x48,
        EOR_IMM = 0x49,
        LSR_ACC = 0x4A,
        JMP_ABS = 0x4C,
        EOR_ABS = 0x4D,
        LSR_ABS = 0x4E,
        BVC_REL = 0x50,
        EOR_IND_Y = 0x51,
        EOR_ZP_X = 0x55,
        LSR_ZP_X = 0x56,
        CLI_IMP = 0x58,
        EOR_ABS_Y = 0x59,
        EOR_ABS_X = 0x5D,
        LSR_ABS_X = 0x5E,
        RTS_IMP = 0x60,
        ADC_X_IND = 0x61,
        ADC_ZP = 0x65,
        ROR_ZP = 0x66,
        PLA_IMP = 0x68,
        ADC_IMM = 0x69,
        ROR_ACC = 0x6A,
        JMP_IND = 0x6C,
        ADC_ABS = 0x6D,
        ROR_ABS = 0x6E,
        BVS_REL = 0x70,
        ADC_IND_Y = 0x71,
        ADC_ZP_X = 0x75,
        ROR_ZP_X = 0x76,
        SEI_IMP = 0x78,
        ADC_ABS_Y = 0x79,
        ADC_ABS_X = 0x7D,
        ROR_ABS_X = 0x7E,
        STA_X_IND = 0x81,
        STY_ZP = 0x84,
        STA_ZP = 0x85,
        STX_ZP = 0x86,
        DEY_IMP = 0x88,
        TXA_IMP = 0x8A,
        STY_ABS = 0x8C,
        STA_ABS = 0x8D,
        STX_ABS = 0x8E,
        BCC_REL = 0x90,
        STA_IND_Y = 0x91,
        STY_ZP_X = 0x94,
        STA_ZP_X = 0x95,
        STX_ZP_Y = 0x96,
        TYA_IMP = 0x98,
        STA_ABS_Y = 0x99,
        TXS_IMP = 0x9A,
        STA_ABS_X = 0x9D,
        LDY_IMM = 0xA0,
        LDA_X_IND = 0xA1,
        LDX_IMM = 0xA2,
        LDY_ZP = 0xA4,
        LDA_ZP = 0xA5,
        LDX_ZP = 0xA6,
        TAY_IMP = 0xA8,
        LDA_IMM = 0xA9,
        TAX_IMP = 0xAA,
        LDY_ABS = 0xAC,
        LDA_ABS = 0xAD,
        LDX_ABS = 0xAE,
        BCS_REL = 0xB0,
        LDA_IND_Y = 0xB1,
        LDY_ZP_X = 0xB4,
        LDA_ZP_X = 0xB5,
        LDX_ZP_Y = 0xB6,
        CLV_IMP = 0xB8,
        LDA_ABS_Y = 0xB9,
        TSX_IMP = 0xBA,
        LDY_ABS_X = 0xBC,
        LDA_ABS_X = 0xBD,
        LDX_ABS_Y = 0xBE,
        CPY_IMM = 0xC0,
        CMP_X_IND = 0xC1,
        CPY_ZP = 0xC4,
        CMP_ZP = 0xC5,
        DEC_ZP = 0xC6,
        INY_IMP = 0xC8,
        CMP_IMM = 0xC9,
        DEX_IMP = 0xCA,
        CPY_ABS = 0xCC,
        CMP_ABS = 0xCD,
        DEC_ABS = 0xCE,
        BNE_REL = 0xD0,
        CMP_IND_Y = 0xD1,
        CMP_ZP_X = 0xD5,
        DEC_ZP_X = 0xD6,
        CLD_IMP = 0xD8,
        CMP_ABS_Y = 0xD9,
        CMP_ABS_X = 0xDD,
        DEC_ABS_X = 0xDE,
        CPX_IMM = 0xE0,
        SBC_X_IND = 0xE1,
        CPX_ZP = 0xE4,
        SBC_ZP = 0xE5,
        INC_ZP = 0xE6,
        INX_IMP = 0xE8,
        SBC_IMM = 0xE9,
        NOP_IMP = 0xEA,
        CPX_ABS = 0xEC,
        SBC_ABS = 0xED,
        INC_ABS = 0xEE,
        BEQ_REL = 0xF0,
        SBC_IND_Y = 0xF1,
        SBC_ZP_X = 0xF5,
        INC_ZP_X = 0xF6,
        SED_IMP = 0xF8,
        SBC_ABS_Y = 0xF9,
        SBC_ABS_X = 0xFD,
        INC_ABS_X = 0xFE
    };

    explicit Cpu(const Memory& program_memory) : mem(program_memory) {
        reset();
        init_opcode_map();
    }

    void reset() {
        PC = 0xfffc;
        SP = 0xfd;
        A = X = Y = 0;
        flags = {0};
    }

    void tick() {
        auto instruction = static_cast<OpCode>(next_byte());

        auto iter = instruction_map.find(instruction);
        if (iter == instruction_map.end()) {
            illegal();
            return;
        }

        printf("Executing instruction: %d\n", static_cast<int>(instruction));
        (this->*iter->second)();
    }

    private:

    Memory mem; // Memory

    std::map<OpCode, void (Cpu::*)()> instruction_map;
    // Registers
    Word PC; // Program Counter
    Byte SP; // Stack Pointer

    Byte A; // Accumulator
    Byte X; // Index Register X
    Byte Y; // Index Register Y

    StatusFlags flags; // Processor Status Flags

    void init_opcode_map() {
        instruction_map[OpCode::LDA_IMM] = &Cpu::lda_immediate;
        instruction_map[OpCode::LDA_ABS] = &Cpu::lda_absolute;
        instruction_map[OpCode::LDA_ABS_X] = &Cpu::lda_absolute_x;
        instruction_map[OpCode::LDA_ABS_Y] = &Cpu::lda_absolute_y;
        instruction_map[OpCode::LDA_ZP] = &Cpu::lda_zero_page;
        instruction_map[OpCode::LDA_ZP_X] = &Cpu::lda_zero_page_x;
        instruction_map[OpCode::LDA_X_IND] = &Cpu::lda_x_indirect;
        instruction_map[OpCode::LDA_IND_Y] = &Cpu::lda_indirect_y;

        instruction_map[OpCode::LDX_IMM] = &Cpu::ldx_immediate;
        instruction_map[OpCode::LDY_IMM] = &Cpu::ldy_immediate;
    }

    // Fetching
    Byte next_byte() {
        Byte result = mem.read(PC);
        ++PC;
        return result;
    }
    Byte peak_next_byte() {
        return mem.read(PC);
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
    void illegal() const {
        printf("Illegal instruction at 0x%04X\n", PC - 1);
        //exit(1);
    }

    void lda_immediate() {
        ld_immediate(A);
    }
    void lda_absolute() {
        ld_absolute(A);
    }
    void lda_absolute_x() {
        ld_absolute_indexed(A, X);
    }
    void lda_absolute_y() {
        ld_absolute_indexed(A, Y);
    }
    void lda_zero_page() {
        ld_zero_page(A);
    }
    void lda_zero_page_x() {
        ld_zero_page_indexed(A, X);
    }
    void lda_x_indirect() {

    }
    void lda_indirect_y() {

    }
    void lda_zero_page_indirect_y() {

    }

    void ldx_immediate() {
        ld_immediate(X);
    }
    void ldx_absolute() {
        ld_absolute(X);
    }

    void ldy_immediate() {
        ld_immediate(Y);
    }
    void ldy_absolute() {
        ld_absolute(Y);
    }
    // Helper functions

    void ld_immediate(Byte& reg) {
        reg = next_byte();
        set_zero_and_negative_flag(reg);
    }
    void ld_absolute(Byte& reg) {
        Word address = next_word_little_endian();
        reg = mem.read(address);
        set_zero_and_negative_flag(reg);
    }
    void ld_absolute_indexed(Byte& reg, const Byte& index) {
        Word address = next_word_little_endian();
        address += index;
        reg = mem.read(address);
        set_zero_and_negative_flag(reg);
    }
    void ld_zero_page(Byte& reg) {
        Byte address = next_byte();
        reg = mem.read(address);
        set_zero_and_negative_flag(reg);
    }
    void ld_zero_page_indexed(Byte& reg, const Byte& index) {
        Byte address = next_byte();
        address += index;
        reg = mem.read(address);
        set_zero_and_negative_flag(reg);
    }
    void ld_zero_page_indexed_indirect(Byte& reg, const Byte index) {
        Byte address = next_byte();
        address = address ^ index;
        reg = mem.read(address);
        set_zero_and_negative_flag(reg);
    }


    void set_zero_and_negative_flag(const Byte& reg) {
        flags.Z = reg == 0;
        flags.N = reg & (1 << 7);
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