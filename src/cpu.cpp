#include "cpu.h"

Cpu::Cpu(const Memory& memory) : mem(memory) {
}

void Cpu::reset() {
    PC = 0;
    SP = 0;
    A = 0;
    X = 0;
    Y = 0;
    StatusFlags = {};
}

void Cpu::tick() {
    const auto option = next_instruction();
    if (!option.has_value()) {
        // TODO: handle error
    }
    OpCode opcode = option.value();
    if (!execute_instruction(opcode)) {
        // TODO: handle error
    }
}

std::optional<OpCode> Cpu::next_instruction() {
    // TODO: Implement
    const auto optional = mem.read(PC*sizeof(Byte));
    return std::nullopt;
}

bool Cpu::execute_instruction(const OpCode &opcode) {
    // TODO: Get the function pointer from the opcode and call it
    return false;
}

bool Cpu::loadIntoRegister(Byte& reg, const Byte& address) {
    const auto option = mem.read(address);
    if (!option.has_value()) {
        return false;
    }

    reg = option.value();
    return true;
}

bool Cpu::storeIntoMemory(const Byte& value, const Byte& address) {
    return mem.write(address, value);
}

bool Cpu::LDA_IMM(const Byte& address) {
    const auto option = mem.read(address);
    if (!option.has_value()) {
        return false;
    }
    A = option.value();
    PC += 2;
    return true;
}

bool Cpu::LDX_IMM(const Byte& address) {
    // TODO: Implement
    return false;
}

bool Cpu::LDY_IMM(const Byte& address) {
    // TODO: Implement
    return false;
}
