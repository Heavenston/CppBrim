
#include <string>
#include <array>
#include <map>
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
    Vec<map<string, u16>*> scopes;

    State(string *er_mess, const Vec<Token> &tkns, Chunk *chnk):
     tokens(tkns), error_message(er_mess), chunk(chnk) {
        create_scope();
    }
    ~State() {
        for (usize i = 0; i < scopes.get_length(); i++) {
            delete scopes[i];
        }
    }

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

    const map<string, u16> *current_scope() {
        return scopes.last();
    }
    const map<string, u16> *create_scope() {
        scopes.push(new map<string, u16>());
        return scopes.last();
    }
};

enum class CompileResult: u8 {
    Ok,
    Invalid,
    InvalidSyntax,
};

struct BlockCompileResult {
    CompileResult compile_result;
    bool return_value;

    BlockCompileResult(CompileResult cr, bool rv = false): compile_result(cr), return_value(rv) {}
};

#define compile_expect_invalid(result) if (result != CompileResult::Ok) {\
    return result;\
}
#define compile_expect(result) if (result != CompileResult::Ok) {\
    return CompileResult::InvalidSyntax;\
}
#define propagate_error(result) if (result == CompileResult::InvalidSyntax) {\
    return CompileResult::InvalidSyntax;\
}

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
CompileResult compile_declaration(State &state);
BlockCompileResult compile_block(State &state);

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

    // Declaration Statement
    {
        CompileResult r = compile_declaration(state);
        propagate_error(r);
        if (r == CompileResult::Ok) {
            compile_consume(TokenType::SemiColon, "';' expected after expression");
            return CompileResult::Ok;
        }
    }

    // Block Statement
    {
        BlockCompileResult r = compile_block(state);
        propagate_error(r.compile_result);
        if (r.compile_result == CompileResult::Ok) {
            if (r.return_value) {
                state.chunk->push_opcode(OpCode::Pop);
            }
            return CompileResult::Ok;
        }
    }

    // Expression Statement
    {
        CompileResult r = compile_expression(state);
        propagate_error(r);
        if (r == CompileResult::Ok) {
            compile_consume(TokenType::SemiColon, "';' expected after declaration");
            return CompileResult::Ok;
        }
    }

    state.error_message = new string("Invalid statement");
    return CompileResult::Invalid;
}

CompileResult compile_declaration(State &state) {
    
    if (state.current() == nullptr) {
        state.error_message = new string("Declaration expected found end of input");
        return CompileResult::Invalid;
    }

    // Declaration
    {
        if (token_is(state.current(), TokenType::Let)) {
            state.next();
            auto name = state.current();
            compile_consume(TokenType::Symbol, "Expected symbol after 'let'");
            auto offset = state.chunk->write_string(name->data.text);
            if (token_is(state.current(), TokenType::Equal)) {
                compile_consume(TokenType::Equal, "");
                compile_expect(compile_expression(state));
            }
            else {
                state.chunk->push_opcode(OpCode::Null);
            }
            state.chunk->push_opcode(OpCode::GlobalDeclaration);
            state.chunk->push_arg(offset);
        }
    }

    return CompileResult::Invalid;
}

BlockCompileResult compile_block(State &state) {
    if (token_is(state.current(), TokenType::CurlyOpen)) {
        state.next();
        state.create_scope();
        bool return_value = false;
        
        for (;;) {
            if (token_is(state.current(), TokenType::CurlyClose)) {
                break;
            }

            CompileResult r = compile_declaration(state);
            if (r == CompileResult::InvalidSyntax) {
                return BlockCompileResult(CompileResult::InvalidSyntax);
            }
            if (r == CompileResult::Ok) {
                if (!token_is(state.next(), TokenType::SemiColon)) {
                    state.error_message = new string("';' expected after declaration");
                    return CompileResult::InvalidSyntax;
                }
                continue;
            }

            r = compile_expression(state);
            if (r != CompileResult::Ok) {
                return BlockCompileResult(CompileResult::InvalidSyntax);
            }

            if (token_is(state.current(), TokenType::SemiColon)) {
                state.next();
                state.chunk->push_opcode(OpCode::Pop);
            }
            else {
                return_value = true;
                break;
            }

        }

        compile_consume(TokenType::CurlyClose, "'}' expected to close block");
        return BlockCompileResult(CompileResult::Ok, return_value);
    }

    state.error_message = new string("Invalid block");
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
                return compile_value(state);
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
        auto offset = state.chunk->write_string(name->data.text);
        compile_consume(TokenType::Equal, "");
        compile_expect(compile_expression(state));
        compile_consume(TokenType::SemiColon, "Expected ';' after assignement");
        state.chunk->push_opcode(OpCode::GlobalAssignement);
        state.chunk->push_arg(offset);
        return CompileResult::Ok;
    }

    // Block Expression
    {
        BlockCompileResult r = compile_block(state);
        propagate_error(r.compile_result);
        if (r.compile_result == CompileResult::Ok) {
            if (!r.return_value) {
                state.chunk->push_opcode(OpCode::Null);
            }
            return CompileResult::Ok;
        }
    }

    return compile_expression_x(state, 255);
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
        usize offset = state.chunk->write_string(current->data.text);
        state.chunk->push_arg(offset);
        return CompileResult::Ok;
    }

    state.stack_has_value = false;
    state.error_message = new string("Invalid Value");
    return CompileResult::Invalid;
}