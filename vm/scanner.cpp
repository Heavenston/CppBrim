
#include <regex>
#include <cstring>
#include <string>
#include <cstdio>
#include <iostream>
#include "common.h"
#include "scanner.h"

using namespace brim;

Token::Token(TokenType _type, TokenData _data) {
    type = _type;
    data = _data;
}

Vec<Token> brim::scan(const char *source) {
    Vec<Token> vec;
    vec.ensure_capacity(strlen(source)/4);

    u32 i = 0;
    while (i < strlen(source)) {
        auto c = &source[i];
        Token *token = nullptr;

        switch (*c) {
            case '=':
                i++;
                if (source[i] == '=') {
                    i++;
                    token = new Token(TokenType::IsEqual);
                }
                else {
                    token = new Token(TokenType::Equal);
                }
            break;
            case '+':
                i++;
                if (source[i] == '=') {
                    i++;
                    token = new Token(TokenType::PlusAssigment);
                }
                else {
                    token = new Token(TokenType::Plus);
                }
            break;
            case '-':
                i++;
                if (source[i] == '=') {
                    i++;
                    token = new Token(TokenType::DashAssigment);
                }
                else {
                    token = new Token(TokenType::Dash);
                }
            break;
            case '*':
                i++;
                if (source[i] == '=') {
                    i++;
                    token = new Token(TokenType::StarAssigment);
                }
                else if (source[i] == '*') {
                    i++;
                    token = new Token(TokenType::Power);
                }
                else {
                    token = new Token(TokenType::Star);
                }
            break;
            case '/':
                i++;
                if (source[i] == '=') {
                    i++;
                    token = new Token(TokenType::SlashAssigment);
                }
                else {
                    token = new Token(TokenType::Slash);
                }
            break;
            case '%':
                i++;
                if (source[i] == '=') {
                    i++;
                    token = new Token(TokenType::Percent);
                }
                else {
                    token = new Token(TokenType::PercentAssigment);
                }
            break;
            case '!':
                i++;
                if (source[i] == '=') {
                    i++;
                    token = new Token(TokenType::NotEqual);
                }
                else {
                    token = new Token(TokenType::BooleanNot);
                }
            break;
            case '>':
                i++;
                if (source[i] == '=') {
                    i++;
                    token = new Token(TokenType::GreaterOrEqualThan);
                }
                else {
                    token = new Token(TokenType::GreaterThan);
                }
            break;
            case '<':
                i++;
                if (source[i] == '=') {
                    i++;
                    token = new Token(TokenType::LessOrEqualThan);
                }
                else {
                    token = new Token(TokenType::LessThan);
                }
            break;
            case '&':
                i++;
                if (source[i] == '&') {
                    i++;
                    token = new Token(TokenType::BooleanAnd);
                }
            break;
            case '|':
                i++;
                if (source[i] == '|') {
                    i++;
                    token = new Token(TokenType::BooleanOr);
                }
            break;
            case ',':
                i++;
                token = new Token(TokenType::Comma);
            break;
            case ';':
                i++;
                token = new Token(TokenType::SemiColon);
            break;
            case ':':
                i++;
                token = new Token(TokenType::Colon);
            break;
            case '{':
                i++;
                token = new Token(TokenType::CurlyOpen);
            break;
            case '}':
                i++;
                token = new Token(TokenType::CurlyClose);
            break;
            case '[':
                i++;
                token = new Token(TokenType::SquareOpen);
            break;
            case ']':
                i++;
                token = new Token(TokenType::SquareClose);
            break;
            case '(':
                i++;
                token = new Token(TokenType::BracketOpen);
            break;
            case ')':
                i++;
                token = new Token(TokenType::BracketClose);
            break;
        }

        if (token == nullptr) {
            if (strcmp((char*)c, "let") == 0) {
                i += 3;
                token = new Token(TokenType::Let);
            }
            else if (strcmp((char*)c, "const") == 0) {
                i += 4;
                token = new Token(TokenType::Const);
            }
            else if (strcmp((char*)c, "return") == 0) {
                i += 6;
                token = new Token(TokenType::Return);
            }
            else if (strcmp((char*)c, "true") == 0) {
                i += 4;
                token = new Token(TokenType::True);
            }
            else if (strcmp((char*)c, "false") == 0) {
                i += 5;
                token = new Token(TokenType::False);
            }
            else if (strcmp((char*)c, "null") == 0) {
                i += 4;
                token = new Token(TokenType::Null);
            }
            else if (strcmp((char*)c, "if") == 0) {
                i += 2;
                token = new Token(TokenType::If);
            }
            else if (strcmp((char*)c, "else") == 0) {
                i += 4;
                token = new Token(TokenType::Else);
            }
            else if (strcmp((char*)c, "while") == 0) {
                i += 5;
                token = new Token(TokenType::While);
            }
            else if (strcmp((char*)c, "func") == 0) {
                i += 4;
                token = new Token(TokenType::Function);
            }
        }

        // Multiline Comment
        if (token == nullptr) {
            std::cmatch results;
            std::regex_search((char*)c, results, std::regex("^#-([^#-]*)-#"));
            if (!results.empty()) {
                i += results.length();
                auto match = results[1].str();
                TokenData data;
                data.text = &match;
                token = new Token(TokenType::MultiLineComment, data);
            }
        }
        // Inline Comment
        if (token == nullptr) {
            std::cmatch results;
            std::regex_search((char*)c, results, std::regex("^#(.*)"));
            if (!results.empty()) {
                i += results.length();
                auto match = results[1].str();
                TokenData data;
                data.text = &match;
                token = new Token(TokenType::InlineComment, data);
            }
        }
        // Strings
        if (token == nullptr) {
            std::cmatch results;
            std::regex_search((char*)c, results, std::regex("^(?:'([^']*)'|\"([^\"]*)\")"));
            if (!results.empty()) {
                i += results.length();
                auto match = results[1].str();
                if (match == "") {
                    match = results[2].str();
                }
                TokenData data;
                data.text = &match;
                token = new Token(TokenType::String, data);
            }
        }

        // Decimal Numbers
        if (token == nullptr) {
            std::cmatch results;
            std::regex_search((char*)c, results, std::regex("^([0-9]+(?:\\.[0-9]+)?)"));
            if (!results.empty()) {
                i += results.length();
                auto match = results[1].str();
                f64 n = std::stof(match);
                TokenData data;
                data.number = n;
                token = new Token(TokenType::Number, data);
            }
        }
        // Hexadecimal numbers
        if (token == nullptr) {
            std::cmatch results;
            std::regex_search((char*)c, results, std::regex("^[+-]?0[xX]([0-9A-Fa-f]+)"));
            if (!results.empty()) {
                i += results.length();
                auto match = results[1].str();
                f64 n = std::stoi(match, nullptr, 16);
                TokenData data;
                data.number = n;
                token = new Token(TokenType::Number, data);
            }
        }
        // Binary numbers
        if (token == nullptr) {
            std::cmatch results;
            std::regex_search((char*)c, results, std::regex("^0[bB]([01]+)"));
            if (!results.empty()) {
                i += results.length();
                auto match = results[1].str();
                f64 n = std::stoi(match, nullptr, 2);
                TokenData data;
                data.number = n;
                token = new Token(TokenType::Number, data);
            }
        }


        if (token == nullptr) {
            vec.push(Token(TokenType::Error));
        }
        else {
            vec.push(*token);
        }
    }

    return vec;
};