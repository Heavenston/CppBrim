#pragma once

#include <cstring>
#include <string>
#include <regex>
#include "common.h"
#include "vec.h"

namespace brim {

    enum class TokenType: u8 {
        Equal = 0,
        Power,
        Star,
        Increment,
        Plus,
        Decrement,
        Dash,
        Slash,
        Percent,

        PlusAssigment,
        DashAssigment,
        StarAssigment,
        SlashAssigment,
        PercentAssigment,

        IsEqual,
        NotEqual,
        GreaterThan,
        LessThan,
        GreaterOrEqualThan,
        LessOrEqualThan,

        BooleanAnd,
        BooleanOr,
        BooleanNot,

        Arrow,

        Comma,
        SemiColon,
        Colon,

        Let,
        Const,
        Return,

        True,
        False,
        Null,
        If,
        Else,
        While,
        Function,
        MultiLineComment,
        InlineComment,

        CurlyOpen,
        CurlyClose,

        SquareOpen,
        SquareClose,

        BracketOpen,
        BracketClose,

        String,

        Number,
        Symbol,

        Error,
    };

    union TokenData {
        f64 number = 0;
        std::string *text;

        ~TokenData() {};
    };

    struct Token {
        TokenType type;
        TokenData data;

        Token(TokenType _type, TokenData _data = TokenData{});
    };

    Vec<Token> scan(const char *source);

}