#pragma once

#include "tokens.h"
#include "ast.h"

#include <map>
#include <string>
#include <vector>

namespace pp_expr
{
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    Expr_t parse();

    Expr_t parse_expr(int prec = 0);

    const Token& advance();
    bool match(TokenType token_type);
    void consume(TokenType token_type);
    bool endof_token() const;

    int cur_op_precedence() const;
private:
    std::vector<Token> tokens_;
    size_t curr_{0};
};
}
