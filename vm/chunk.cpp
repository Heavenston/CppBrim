
#include <cstring>
#include "common.h"
#include "chunk.h"

using namespace brim;

usize Chunk::push_opcode(u8 opcode) {
    program.push(opcode);
    return program.get_length()-1;
}
usize Chunk::push_arg(usize value) {
    u8 *last = (u8*)(&program.last())+1;
    program.ensure_capacity(program.get_length()+sizeof(usize));
    *last = value;
    program.set_length(program.get_length()+sizeof(usize));
    return 0;
}

void Chunk::write_opcode(u8 opcode, usize offset) {
    program[offset] = opcode;
}
void Chunk::write_arg(usize value, usize offset) {
    u8 *pointer = (u8*)(&program.first())+offset;
    program.ensure_capacity(offset+sizeof(usize));
    *pointer = value;
    program.set_length(offset+sizeof(usize));
}

u8 Chunk::get_opcode(usize offset) const {
    return program[offset];
}
usize Chunk::get_arg(usize offset) const {
    u8 *pointer = (u8*)(&program.first())+offset;
    usize value = *((usize*)(pointer));
    return value;
}

usize Chunk::get_program_length() const {
    return program.get_length();
}

usize Chunk::write_string(char *string, usize length) {
    values.ensure_capacity(values.get_length()+length);
    auto offset = values.get_length();
    const char* last = (char*)(&values.last()) + sizeof(char);
    strcpy(string, last);
    return offset;
}
const char *Chunk::read_string(usize offset, usize length) const {
    return (char*)(values.get_pointer() + (offset * sizeof(u8)));
}
