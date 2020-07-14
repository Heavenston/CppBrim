
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <cmath>

#include "common.h"
#include "vm.h"
#include "scanner.h"
#include "compiler.h"
#include "opcode.h"
#include "objects/string.h"

using namespace brim;
using namespace std;

VM::VM() {};

void VM::interpret(const char *program) {
    Vec<Token> tokens = scan(program);
    chunk = compile(tokens);
    chunk->debug();
    ip = 0;
    stack.set_capacity(0);
    run();
}
void VM::interpret_file(const char *path) {
    ifstream t(path);
    string str((istreambuf_iterator<char>(t)),
                istreambuf_iterator<char>());
    
    interpret(str.c_str());
}

void VM::run() {
    while (ip < chunk->get_program_length()) {

        OpCode v = chunk->get_opcode(ip);
        if (v == OpCode::Number) {
            Value v(*(chunk->get_arg<f64>(ip+1)));
            ip += sizeof(f64);
            stack.push(v);
        }
        else if (v == OpCode::String) {
            usize string_offset = *(chunk->get_arg<f64>(ip+1));
            const char* string = chunk->read_string(string_offset);
            ip += sizeof(f64);
            stack.push(Value(new StringObj(string)));
        }
        else if (v == OpCode::Add) {
            Value &b = stack.pop();
            Value &a = stack.pop();
            if (!a.is_type(ValueType::Number) || !b.is_type(ValueType::Number)) {
                return;
            }
            stack.push(Value(a.number_val() + b.number_val()));
        }
        else if (v == OpCode::Sub) {
            Value &b = stack.pop();
            Value &a = stack.pop();
            if (!a.is_type(ValueType::Number) || !b.is_type(ValueType::Number)) {
                return;
            }
            stack.push(Value(a.number_val() - b.number_val()));
        }
        else if (v == OpCode::Mul) {
            Value &b = stack.pop();
            Value &a = stack.pop();
            if (!a.is_type(ValueType::Number) || !b.is_type(ValueType::Number)) {
                return;
            }
            stack.push(Value(a.number_val() * b.number_val()));
        }
        else if (v == OpCode::Div) {
            Value &b = stack.pop();
            Value &a = stack.pop();
            if (!a.is_type(ValueType::Number) || !b.is_type(ValueType::Number)) {
                return;
            }
            stack.push(Value(a.number_val() / b.number_val()));
        }
        else if (v == OpCode::Pow) {
            Value &b = stack.pop();
            Value &a = stack.pop();
            if (!a.is_type(ValueType::Number) || !b.is_type(ValueType::Number)) {
                return;
            }
            stack.push(Value(powf(a.number_val(), b.number_val())));
        }

        else if (v == OpCode::Pop) {
            stack.pop();
        }

        ip++;
    }

    cout << "Top of stack: " << stack[0].number_val() << "\n";
}