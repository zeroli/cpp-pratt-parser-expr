#pragma once

#include "tokens.h"

#include <string>
#include <memory>
#include <ostream>

namespace pp_expr
{
struct Expr {
    virtual ~Expr() = default;

    virtual std::ostream& visit(std::ostream& os) const = 0;
};

using Expr_t = std::shared_ptr<Expr>;

inline std::ostream& operator <<(std::ostream& os, const Expr& ast)
{
    return ast.visit(os);
}

struct Number : public Expr {
    Number(double value) : value_(value) {}

    double value() const { return value_; }

    std::ostream& visit(std::ostream& os) const override {
        return os << value();
    }

    double value_;
};

struct Ident : public Expr {
    Ident(const std::string& value) : value_(value) { }

    const std::string& value() const { return value_; }
    std::ostream& visit(std::ostream& os) const override {
        return os << value();
    }

    std::string value_;
};

struct UnaryExpr : public Expr {
    UnaryExpr(const Token& op, const Expr_t& operand)
        : op_(op), operand_(operand)
    {}
    const Token& op() const { return op_; }
    const Expr_t& operand() const { return operand_; }

    std::ostream& visit(std::ostream& os) const override {
        return os << "(" << op().lexeme << " " << *operand() << ")";
    }

    Token op_;
    Expr_t operand_;
};

/// postfix unary operation, for example: a++, b--
struct PostfixUnaryExpr : public UnaryExpr {
    PostfixUnaryExpr(const Token& op, const Expr_t& operand)
        : UnaryExpr(op, operand)
    {}

    std::ostream& visit(std::ostream& os) const override {
        return os << "(" << *operand() << " " << op().lexeme << ")";
    }
};

struct BinaryExpr : public Expr {
    BinaryExpr(const Token& op, const Expr_t& left, const Expr_t& right)
        : op_(op), left_(left), right_(right)
    {}
    const Token& op() const { return op_; }
    const Expr_t& left() const { return left_; }
    const Expr_t& right() const { return right_; }

    std::ostream& visit(std::ostream& os) const override {
        return os << "(" << op().lexeme
                  << " " << *left()
                  << " " << *right()
                  << ")";
    }

    Token op_;
    Expr_t left_;
    Expr_t right_;
};

struct TenaryExpr : public Expr {
    TenaryExpr(const Token& op, const Expr_t& operand1, const Expr_t& operand2, const Expr_t& operand3)
        : op_(op), operand1_(operand1), operand2_(operand2), operand3_(operand3)
    {}
    const Token& op() const { return op_; }
    const Expr_t& operand1() const { return operand1_; }
    const Expr_t& operand2() const { return operand2_; }
    const Expr_t& operand3() const { return operand3_; }

    std::ostream& visit(std::ostream& os) const override {
        return os << "(" << op().lexeme
                  << " " << *operand1()
                  << " " << *operand2()
                  << " " << *operand3()
                  << ")";
    }

    Token op_;
    Expr_t operand1_;
    Expr_t operand2_;
    Expr_t operand3_;
};

template <typename T, typename... Args>
inline Expr_t MakeExpr(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}  // namespace pp_expr
