#pragma once

#include "common.h"
#include "vec.h"

namespace brim {

    class Chunk {
    private:
        Vec<u8> program;
        Vec<u8> values;

    public:
        usize push_opcode(u8 opcode);
        usize push_arg(usize value);
        void write_opcode(u8 opcode, usize offset);
        void write_arg(usize value, usize offset);
        u8 get_opcode(usize offset) const;
        usize get_arg(usize offset) const;

        usize get_program_length() const;

        usize write_string(char *string, usize length);
        const char *read_string(usize offset, usize length) const;
    };

} // namespace brim