#pragma once

#include <string>
#include <cassert>

namespace pp_expr
{
enum TokenType {
    TOK_PLUS = 0,   // +
    TOK_MINUS,      // -
    TOK_STAR,       // *
    TOK_SLASH,      // /
    TOK_AMPERSAND,  // &
    TOK_INC,        // ++
    TOK_DEC,        // --
    TOK_ASSIGN,     // =
    TOK_EQ,         // ==
    TOK_NE,         // !=
    TOK_LT,         // <
    TOK_GT,         // >
    TOK_LE,         // <=
    TOK_GE,         // >=
    TOK_AND,        // &&
    TOK_OR,         // ||
    TOK_QUESTION,   // ?
    TOK_COLON,      // :
    TOK_LPAREN,     // (
    TOK_RPAREN,     // )
    TOK_LSQUAR,     // [
    TOK_RSQUAR,     // ]
    TOK_NUM,        // Number
    TOK_ID,         // Identifier
};

struct Token {
    TokenType token_type;
    std::string lexeme;
};

inline const char* Lexeme(TokenType token_type)
{
    switch (token_type) {
    case TOK_PLUS: return "+";
    case TOK_MINUS: return "-";
    case TOK_STAR: return "*";
    case TOK_SLASH: return "/";
    case TOK_AMPERSAND: return "&";
    case TOK_INC: return "++";
    case TOK_DEC: return "--";
    case TOK_ASSIGN: return "=";
    case TOK_EQ: return "==";
    case TOK_NE: return "!=";
    case TOK_LT: return "<";
    case TOK_GT: return ">";
    case TOK_LE: return "<=";
    case TOK_GE: return ">=";
    case TOK_AND: return "&&";
    case TOK_OR: return "||";
    case TOK_QUESTION: return "?";
    case TOK_COLON: return ":";
    case TOK_LPAREN: return "(";
    case TOK_RPAREN: return ")";
    case TOK_LSQUAR: return "[";
    case TOK_RSQUAR: return "]";
    default:
        assert(0);
    }
    return "";
}
}  // namespace pp_expr
