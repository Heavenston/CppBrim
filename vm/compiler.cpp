
#include <string>
#include <array>
#include "compiler.h"
#include "chunk.h"
#include "vec.h"
#include "opcode.h"

using namespace brim;
using namespace std;

bool token_is(const Token *token, TokenType type) {
    if (token == nullptr) return false;
    return token->type == type;
}

struct State {
    string *error_message = nullptr;

    const Vec<Token> &tokens;
    usize current_offset = 0;

    Chunk *chunk;

    bool stack_has_value = false;

    State(string *er_mess, const Vec<Token> &tkns, Chunk *chnk):
     tokens(tkns), error_message(er_mess), chunk(chnk) {}

    const Token *current() const {
        if (current_offset >= tokens.get_length()) return nullptr;
        return &tokens[current_offset];
    }
    const Token *peek(usize i) const {
        if (current_offset+i >= tokens.get_length()) return nullptr;
        return &tokens[current_offset+i];
    }
    const Token *next() {
        if (current_offset >= tokens.get_length()) return nullptr;
        return &tokens[current_offset++];
    }
};

enum class CompileResult: u8 {
    Ok,
    Invalid,
    InvalidSyntax,
};

#define compile_expect_invalid(result) propagate_error(result); if (result != CompileResult::Ok) {\
    return CompileResult::Invalid;\
}
#define compile_expect(result) if (result != CompileResult::Ok) {\
    return CompileResult::InvalidSyntax;\
}
// #define propagate_error(result) if (result == CompileResult::InvalidSyntax) {\
//     return CompileResult::InvalidSyntax;\
// }
#define propagate_error(result)

#define compile_consume(token, error) {\
    auto next = state.next();\
    if (next == nullptr || next->type != token) {\
        state.error_message = new string(error);\
        return CompileResult::InvalidSyntax;\
    }\
}

CompileResult compile_expression(State &state);
CompileResult compile_value(State &state);
CompileResult compile_statement(State &state);

Chunk *brim::compile(const Vec<Token> &tokens) {
    State state(nullptr, tokens, new Chunk());

    CompileResult r = CompileResult::Ok;
    while ((r = compile_statement(state)) == CompileResult::Ok) {}

    if (r == CompileResult::InvalidSyntax) {
        printf("Invalid Syntax: %s\n", state.error_message->c_str());
        throw;
    }

    return state.chunk;
}

u8 *get_operator_precedence(const TokenType &token) {
    switch (token) {
        case TokenType::Star:
            return new u8[2]{1, (u8)OpCode::Mul};
        break;
        case TokenType::Slash:
            return new u8[2]{1, (u8)OpCode::Div};
        break;
        case TokenType::Power:
            return new u8[2]{1, (u8)OpCode::Pow};
        break;
        case TokenType::Percent:
            return new u8[2]{1, (u8)OpCode::Mod};
        break;

        case TokenType::Plus:
            return new u8[2]{2, (u8)OpCode::Add};
        break;
        case TokenType::Dash:
            return new u8[2]{2, (u8)OpCode::Sub};
        break;
        default:
            return nullptr;
        break;
    }
}

CompileResult compile_statement(State &state) {
    
    if (state.current() == nullptr) {
        state.error_message = new string("Statement expected found end of input");
        return CompileResult::Invalid;
    }

    // Declaration
    {
        if (token_is(state.current(), TokenType::Let)) {
            state.next();
            auto name = state.current();
            compile_consume(TokenType::Symbol, "Expected symbol after 'let'");
            auto offset = state.chunk->write_string(*name->data.text);
            if (token_is(state.current(), TokenType::Equal)) {
                compile_consume(TokenType::Equal, "");
                compile_expression(state);
            }
            else {
                state.chunk->push_opcode(OpCode::Null);
            }
            compile_consume(TokenType::SemiColon, "Expected ';' after declaration");
            state.chunk->push_opcode(OpCode::GlobalDeclaration);
            state.chunk->push_arg(offset);
        }
    }

    // Expression statement
    {
        auto rs = compile_expression(state);
        propagate_error(rs);
        if (rs == CompileResult::Ok) {
            compile_consume(TokenType::SemiColon, "Expected ';' after expression");
            state.chunk->push_opcode(OpCode::Pop);
            state.stack_has_value = false;
            return CompileResult::Ok;
        }
    }

    return CompileResult::Invalid;
}

CompileResult compile_expression_x(State &state, u8 precedence) {
    if (precedence >= 3) precedence = 3;

    if (state.current() == nullptr) {
        state.error_message = new string("Expression expected found end of input");
        return CompileResult::Invalid;
    }

    switch (state.current()->type) {
        case TokenType::BracketOpen:
            state.next();
            compile_expect(compile_expression(state));
            compile_consume(TokenType::BracketClose, "Expect ')' after expression");
        break;
        case TokenType::Dash:
            state.next();
            compile_expect(compile_expression_x(state, 0));
            state.chunk->push_opcode(OpCode::Neg);
            return CompileResult::Ok;
        break;
        default:
            if (precedence == 0) {
                compile_expect_invalid(compile_value(state));
                return CompileResult::Ok;
            }
            compile_expect(compile_expression_x(state, precedence-1));
        break;
    }

    const Token *op = state.current();
    if (op == nullptr) return CompileResult::Ok;

    u8 *op_prep = get_operator_precedence(op->type);
    if (op_prep == nullptr) return CompileResult::Ok;

    if (op_prep[0] <= precedence) {
        state.next();
        compile_expect(compile_expression_x(state, precedence-1));
        state.chunk->push_opcode((OpCode)op_prep[1]);
    }

    return CompileResult::Ok;
}

CompileResult compile_expression(State &state) {

    // Assignement
    if (token_is(state.current(), TokenType::Symbol) && token_is(state.peek(1), TokenType::Equal)) {
        state.next();
        auto name = state.current();
        compile_consume(TokenType::Symbol, "");
        auto offset = state.chunk->write_string(*name->data.text);
        compile_consume(TokenType::Equal, "");
        compile_expression(state);
        compile_consume(TokenType::SemiColon, "Expected ';' after assignement");
        state.chunk->push_opcode(OpCode::GlobalAssignement);
        state.chunk->push_arg(offset);
        return CompileResult::Ok;
    }

    compile_expect_invalid(compile_expression_x(state, 255));
    return CompileResult::Ok;
}

CompileResult compile_value(State &state) {

    if (state.current() == nullptr) {
        state.error_message = new string("Value expected found end of input");
        return CompileResult::Invalid;
    }

    const Token *current = state.current();
    state.stack_has_value = true;
    if (current->type == TokenType::Number) {
        state.next();
        state.chunk->push_opcode(OpCode::Number);
        state.chunk->push_arg<f64>(current->data.number);
        return CompileResult::Ok;
    }
    else if (current->type == TokenType::Null) {
        state.next();
        state.chunk->push_opcode(OpCode::Null);
        return CompileResult::Ok;
    }
    else if (current->type == TokenType::True) {
        state.next();
        state.chunk->push_opcode(OpCode::True);
        return CompileResult::Ok;
    }
    else if (current->type == TokenType::False) {
        state.next();
        state.chunk->push_opcode(OpCode::False);
        return CompileResult::Ok;
    }
    else if (current->type == TokenType::String) {
        state.next();
        state.chunk->push_opcode(OpCode::String);
        usize offset = state.chunk->write_string(current->data.text->c_str());
        state.chunk->push_arg(offset);
        return CompileResult::Ok;
    }

    state.stack_has_value = false;
    state.error_message = new string("Invalid Value");
    return CompileResult::Invalid;
}