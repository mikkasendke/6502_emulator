#include "memory.h"

#include <algorithm>
#include <optional>

Memory::Memory(const Word size_in_bytes) :
    size(size_in_bytes),
    data(new Byte[size_in_bytes]) {
    std::fill_n(data, size, 0);
}

Memory::~Memory() {
    delete[] data;
}

[[nodiscard]]
std::optional<Byte> Memory::read(const Word address) const {
    if (address >= size) return std::nullopt; // handle error better
    return data[address];
}

[[nodiscard]]
bool Memory::write(const Word address, const Byte value) {
    if (address >= size) return false;
    data[address] = value;
    return true;
}
