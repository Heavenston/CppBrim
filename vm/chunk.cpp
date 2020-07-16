
#include <cstring>
#include <string>
#include "common.h"
#include "chunk.h"
#include "opcode.h"

using namespace brim;
using namespace std;

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

usize Chunk::write_string(const char *string) {
    values.ensure_capacity(values.get_length()+strlen(string)+1);
    auto offset = values.get_length();
    char* last = (char*)&values[offset];
    strcpy(last, string);
    values.set_length(values.get_length()+strlen(string)+1);
    return offset;
}
usize Chunk::write_string(const string& string) {
    return write_string(string.c_str());
}
const char *Chunk::read_string(usize offset) const {
    return (const char*)&values[offset];
}

void Chunk::debug() {
    {
        char *strings = (char *)malloc(values.get_length());
        memcpy(strings, &values[0], values.get_length());
        for (usize i = 0; i < values.get_length(); i++) {
            if (strings[i] == 0) {
                strings[i] = 32; 
            }
        }
        printf("Strings: %s\n", strings);
    }
    printf("Program: \n");
    for (usize i = 0; i < program.get_length(); i++) {
        OpCode op = (OpCode)program[i];
        if (OpCode::Number == op) {
            f64 n = *get_arg<f64>(i+1);
            printf(" | Number(%lf)\n", n);
            i += sizeof(f64);
        }
        else if (OpCode::String == op) {
            usize offset = *get_arg<usize>(i+1);
            const char *string = read_string(offset);
            printf(" | String(%s)\n", string);
            i += sizeof(usize);
        }
        else if (OpCode::GlobalDeclaration == op) {
            usize offset = *get_arg<usize>(i+1);
            const char *string = read_string(offset);
            printf(" | GlobalDecl(%s)\n", string);
            i += sizeof(usize);
        }
        else if (OpCode::GlobalAssignement == op) {
            usize offset = *get_arg<usize>(i+1);
            const char *string = read_string(offset);
            printf(" | GlobalAssi(%s)\n", string);
            i += sizeof(usize);
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
        else if (OpCode::Pop == op) {
            printf(" | Pop\n");
        }
        else if (OpCode::Peek == op) {
            usize n = *get_arg<usize>(i+1);
            printf(" | Peek(%lu)\n", n);
            i += sizeof(usize);
        }
        else {
            printf(" | %d\n", program[i]);
        }
    }
    printf("\n");
}
