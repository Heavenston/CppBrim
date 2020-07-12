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
            auto offset = program.get_length();
            write_arg<T>(value, program.get_length());
            return offset;
        }

        void write_opcode(u8 opcode, usize offset);
        template<class T>
        void write_arg(T value, usize offset) {
            program.ensure_capacity(offset+sizeof(T));
            T *v = (T*)(&program[offset]);
            *v = value;
            program.set_length(offset+sizeof(T));
        }

        OpCode get_opcode(usize offset) const;
        template<class T>
        const T *get_arg(usize offset) const {
            const T *value = (T*)(&program[offset]);
            return value;
        }

        usize get_program_length() const;

        usize write_string(const char *string);
        usize write_string(const std::string &string);
        const char *read_string(usize offset) const;

        void debug();
    };

} // namespace brim