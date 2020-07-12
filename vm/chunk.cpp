
#include <cstring>
#include "common.h"
#include "chunk.h"
#include "opcode.h"

using namespace brim;

usize Chunk::push_opcode(OpCode opcode) {
    program.push((u8)opcode);
    return program.get_length()-1;
}
void Chunk::write_opcode(u8 opcode, usize offset) {
    program[offset] = opcode;
}

OpCode Chunk::get_opcode(usize offset) const {
    return (OpCode)program[offset];
}

usize Chunk::get_program_length() const {
    return program.get_length();
}

usize Chunk::write_string(const char *string, usize length) {
    values.ensure_capacity(values.get_length()+length);
    auto offset = values.get_length();
    char* last = (char*)(&values.last()) + sizeof(char);
    strcpy(last, string);
    return offset;
}
const char *Chunk::read_string(usize offset, usize length) const {
    return (char*)(values.get_pointer() + (offset * sizeof(u8)));
}

void Chunk::debug() {
    printf("Program: \n");
    for (usize i = 0; i < program.get_length(); i++) {
        OpCode op = (OpCode)program[i];
        if (OpCode::Number == op) {
            f64 n = *get_arg<f64>(i+1);
            printf(" | Number(%lf)\n", n);
            i += sizeof(f64);
        }
        else if (OpCode::Add == op) {
            printf(" | Add\n");
        }
        else if (OpCode::Div == op) {
            printf(" | Div\n");
        }
        else if (OpCode::Mul == op) {
            printf(" | Mul\n");
        }
        else if (OpCode::Sub == op) {
            printf(" | Sub\n");
        }
        else {
            printf(" | %x\n", program[i]);
        }
    }
    printf("\n");
}
