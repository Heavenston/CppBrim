
#include <cstring>
#include "brim.h"
#include "chunk.h"

using namespace brim;

usize Chunk::push_byte(u8 opcode) {
    program.push(opcode);
    return program.get_length()-1;
}
usize Chunk::write_byte(u8 opcode, usize offset) {
    program[offset] = opcode;
}
u8 Chunk::get_byte(usize offset) {
    return program[offset];
}
usize Chunk::get_program_length() {
    return program.get_length();
}

usize Chunk::write_string(char *string, usize length) {
    values.ensure_capacity(values.get_length()+length);
    auto offset = values.get_length();
    const char* last = (char*)(&values.last()) + sizeof(char);
    strcpy(string, last);
    return offset;
}
char *Chunk::read_string(usize offset, usize length) {
    return (char*)(values.get_pointer() + (offset * sizeof(u8)));
}
