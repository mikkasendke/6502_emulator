#include "memory.h"

Memory::Memory(Word size_in_bytes) :
    size(size_in_bytes),
    data(new Byte[size_in_bytes]) {
    initialize();
}

Memory::~Memory() {
    delete[] data;
}

void Memory::initialize() {
    for (Word i = 0; i < size; ++i) {
        data[i] = 0;
    }
}

Byte Memory::read(Word address) const {
    if (address >= size) return 0; // handle error better
    return data[address];
}

bool Memory::write(Word address, Byte value) {
    if (address >= size) return false;
    data[address] = value;
    return true;
}
