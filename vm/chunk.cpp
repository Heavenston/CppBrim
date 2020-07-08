
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
