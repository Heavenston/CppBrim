#pragma once

#include "brim.h"
#include "vec.h"

namespace brim {
    
    class Chunk {
    private:
        Vec<u8> program;
        Vec<u8> values;

    public:
        usize push_byte(u8 opcode);
        void write_byte(u8 opcode, usize offset);
        u8 get_byte(usize offset);

        usize get_program_length();

        usize write_string(char *string, usize length);
        char *read_string(usize offset, usize length);

    };

}