#pragma once

#include <optional>

#include "types.h"

class Memory {
    public:
        explicit Memory(Word size_in_bytes);
        ~Memory();

        void initialize() const;
        [[nodiscard]] std::optional<Byte> read(Word address) const;
        [[nodiscard]] bool write(Word address, Byte value) const; // returns false if it fails

    private:
        Word size;
        Byte* data;
};
