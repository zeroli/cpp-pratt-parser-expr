#pragma once

#include "tokens.h"
#include <map>

namespace pp_expr
{
static int PP = 1;
static std::map<TokenType, int> binary_op_precedences = {
    { TOK_ASSIGN,   PP++ },
    { TOK_QUESTION, PP++ },
    { TOK_OR,       PP++ },
    { TOK_AND,      PP++ },
    { TOK_EQ,       PP   },
    { TOK_NE,       PP++ },
    { TOK_LT,       PP   },
    { TOK_LE,       PP   },
    { TOK_GT,       PP   },
    { TOK_GE,       PP++ },
    { TOK_PLUS,     PP   },
    { TOK_MINUS,    PP++ },
    { TOK_STAR,     PP   },
    { TOK_SLASH,    PP++ },
    { TOK_LPAREN,   PP   },
    { TOK_LSQUAR,   PP   },
    { TOK_INC,      PP   },
    { TOK_DEC,      PP   },
};

static std::map<TokenType, int> unary_op_precedences = {
    { TOK_PLUS,         PP },
    { TOK_MINUS,        PP },
    { TOK_STAR,         PP },
    { TOK_AMPERSAND,    PP },
    { TOK_INC,          PP },
    { TOK_DEC,          PP },
};

inline int get_precedence(const TokenType& token_type)
{
    return binary_op_precedences[token_type];
}
}
