#pragma once

#include "types.h"

class Memory {
    public:
        explicit Memory(Word size_in_bytes);
        ~Memory();

        void initialize();
        [[nodiscard]] Byte read(Word address) const;
        bool write(Word address, Byte value); // returns false if it fails

    private:
        Word size;
        Byte* data;
};
