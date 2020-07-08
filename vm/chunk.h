#pragma once

#include <string>
#include "common.h"
#include "vec.h"
#include "opcode.h"

namespace brim {

    class Chunk {
    private:
        Vec<u8> program;
        Vec<u8> values;

    public:
        usize push_opcode(OpCode opcode);
        template<class T>
        usize push_arg(T value) {
            u8 *last = (u8*)(&program.last())+1;
            program.ensure_capacity(program.get_length()+sizeof(T));
            *last = value;
            program.set_length(program.get_length()+sizeof(T));
            return 0;
        }

        void write_opcode(u8 opcode, usize offset);
        template<class T>
        void write_arg(T value, usize offset) {
            u8 *pointer = (u8*)(&program.first())+offset;
            program.ensure_capacity(offset+sizeof(T));
            *pointer = value;
            program.set_length(offset+sizeof(T));
        }

        OpCode get_opcode(usize offset) const;
        template<class T>
        T get_arg(T offset) const {
            u8 *pointer = (u8*)(&program.first())+offset;
            T value = *((T*)(pointer));
            return value;
        }

        usize get_program_length() const;

        usize write_string(const char *string, usize length);
        const char *read_string(usize offset, usize length) const;
    };

} // namespace brim