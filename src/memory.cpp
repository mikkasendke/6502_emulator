#include "memory.h"

#include <optional>

Memory::Memory(const Word size_in_bytes) :
    size(size_in_bytes),
    data(new Byte[size_in_bytes]) {
    initialize();
}

Memory::~Memory() {
    delete[] data;
}

void Memory::initialize() const {
    for (Word i = 0; i < size; ++i) {
        data[i] = 0;
    }
}

[[nodiscard]]
std::optional<Byte> Memory::read(const Word address) const {
    if (address >= size) return std::nullopt; // handle error better
    return data[address];
}

[[nodiscard]]
bool Memory::write(const Word address, const Byte value) const {
    if (address >= size) return false;
    data[address] = value;
    return true;
}
