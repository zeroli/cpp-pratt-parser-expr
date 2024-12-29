#include "parser.h"
#include "precedence.h"

#include <cassert>

namespace pp_expr
{
typedef Expr_t (*PrefixParser_t)(Parser& parser, const Token&);
typedef Expr_t (*InfixParser_t)(Parser& parser, const Token&, const Expr_t& left);

static Expr_t parse_unary_expr(Parser& parser, const Token& op_token)
{
    return MakeExpr<UnaryExpr>(
        op_token,
        /// right associative,
        /// -1 to make following prefix op have higher precedence,
        /// and bind to the operand
        parser.parse_expr(unary_op_precedences[op_token.token_type] - 1)
    );
}

static Expr_t parse_ident(Parser& parser, const Token& token)
{
    return MakeExpr<Ident>(token.lexeme);
}

static Expr_t parse_num(Parser& parser, const Token& token)
{
    return MakeExpr<Number>(strtod(token.lexeme.c_str(), nullptr));
}

static Expr_t parse_lparen_expr(Parser& parser, const Token& op_token)
{
    auto expr = parser.parse_expr(0);
    parser.consume(TOK_RPAREN);
    return expr;
}

static std::map<TokenType, PrefixParser_t> PrefixParsers =
{
    { TOK_ID,        &parse_ident       },
    { TOK_NUM,       &parse_num         },
    { TOK_INC,       &parse_unary_expr  },
    { TOK_DEC,       &parse_unary_expr  },
    { TOK_PLUS,      &parse_unary_expr  },
    { TOK_MINUS,     &parse_unary_expr  },
    { TOK_STAR,      &parse_unary_expr  },
    { TOK_AMPERSAND, &parse_unary_expr  },
    { TOK_LPAREN,    &parse_lparen_expr },
};

/// parse binary op expression for op with left associative
static Expr_t parse_binary_expr_left(Parser& parser, const Token& op_token, const Expr_t& left)
{
    return MakeExpr<BinaryExpr>(
        op_token,
        left,
        parser.parse_expr(get_precedence(op_token.token_type))
    );
}

/// parse binary op expression for op with right associative
static Expr_t parse_binary_expr_right(Parser& parser, const Token& op_token, const Expr_t& left)
{
    return MakeExpr<BinaryExpr>(
        op_token,
        left,
        /// right associative for assignment: a = b = c => a = (b = c)
        /// -1 to make following prefix op have higher precedence,
        /// and bind to the operand
        parser.parse_expr(get_precedence(op_token.token_type) - 1)
    );
}

static Expr_t parse_question_expr(Parser& parser, const Token& op_token, const Expr_t& left)
{
    auto true_expr = parser.parse_expr(0);
    parser.consume(TOK_COLON);
    auto false_expr = parser.parse_expr(0);
    return MakeExpr<TenaryExpr>(
        op_token,
        left,
        true_expr,
        false_expr
    );
}

/// parse x[n]
static Expr_t parse_lsquar_expr(Parser& parser, const Token& op_token, const Expr_t& left)
{
    auto expr = MakeExpr<BinaryExpr>(
        op_token,
        left,
        parser.parse_expr(0)
    );
    parser.consume(TOK_RSQUAR);
    return expr;
}

/// parse x++/x--/x!
static Expr_t parse_post_unary_expr(Parser& parser, const Token& op_token, const Expr_t& left)
{
    auto expr = MakeExpr<PostfixUnaryExpr>(
        op_token,
        left
    );
    return expr;
}

static std::map<TokenType, InfixParser_t> InfixParsers =
{
    { TOK_ASSIGN,   &parse_binary_expr_right    },
    { TOK_QUESTION, &parse_question_expr        },
    { TOK_PLUS,     &parse_binary_expr_left     },
    { TOK_MINUS,    &parse_binary_expr_left     },
    { TOK_STAR,     &parse_binary_expr_left     },
    { TOK_SLASH,    &parse_binary_expr_left     },
    { TOK_EQ,       &parse_binary_expr_left     },
    { TOK_NE,       &parse_binary_expr_left     },
    { TOK_LT,       &parse_binary_expr_left     },
    { TOK_LE,       &parse_binary_expr_left     },
    { TOK_GT,       &parse_binary_expr_left     },
    { TOK_GE,       &parse_binary_expr_left     },
    { TOK_AND,      &parse_binary_expr_left     },
    { TOK_OR,       &parse_binary_expr_left     },
    { TOK_LSQUAR,   &parse_lsquar_expr          },
    { TOK_INC,      &parse_post_unary_expr      },
    { TOK_DEC,      &parse_post_unary_expr      },
};

Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens)
{}

Expr_t Parser::parse()
{
    Expr_t ast;
    while (!endof_token()) {
        ast = parse_expr();
        break;
    }
    return ast;
}

Expr_t Parser::parse_expr(int prec)
{
    auto tok = advance();
    auto prefix = PrefixParsers[tok.token_type];
    assert(prefix);
    auto left = (*prefix)(*this, tok);

    while (!endof_token() && cur_op_precedence() > prec)
    {
        auto tok = advance();
        auto infix = InfixParsers[tok.token_type];
        if (!infix) {
            break;
        }
        left = (*infix)(*this, tok, left);
    }
    return left;
}

const Token& Parser::advance()
{
    assert(!endof_token() && "no more token");
    return tokens_[curr_++];
}

bool Parser::match(TokenType token_type)
{
    if (endof_token()) {
        return false;
    }
    if (tokens_[curr_].token_type != token_type) {
        return false;
    }
    advance();
    return true;
}

void Parser::consume(TokenType token_type)
{
    if (!match(token_type)) {
        fprintf(stderr, "Error: expected token '%s', got token '%s'\n",
            Lexeme(token_type), tokens_[curr_].lexeme.c_str());
        assert(0);
    }
}

bool Parser::endof_token() const
{
    return curr_ >= tokens_.size();
}

int Parser::cur_op_precedence() const
{
    return get_precedence(tokens_[curr_].token_type);
}
}  // namespace pp_expr
