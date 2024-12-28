#include "parser.h"

#include <iostream>
#include <cassert>
#include <sstream>

using namespace pp_expr;

int main(int argc, char** argv)
{
    {
        std::vector<Token> tokens = {
            { TOK_MINUS, "-" },
            { TOK_ID, "a" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(- a)");
    }
    {
        std::vector<Token> tokens = {
            { TOK_MINUS, "-" },
            { TOK_ID, "10" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(- 10)");
    }
    {
        /// 3 + 4 - 5 - 6
        /// ((3 + 4) - 5) - 6
        std::vector<Token> tokens = {
            { TOK_NUM, "3" },
            { TOK_PLUS, "+" },
            { TOK_NUM, "4" },
            { TOK_MINUS, "-" },
            { TOK_NUM, "5" },
            { TOK_MINUS, "-" },
            { TOK_NUM, "6" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(- (- (+ 3 4) 5) 6)");
    }
    {
        /// 3 + (4 - 5) - 6
        std::vector<Token> tokens = {
            { TOK_NUM, "3" },
            { TOK_PLUS, "+" },
            { TOK_LPAREN, "(" },
            { TOK_NUM, "4" },
            { TOK_MINUS, "-" },
            { TOK_NUM, "5" },
            { TOK_RPAREN, ")" },
            { TOK_MINUS, "-" },
            { TOK_NUM, "6" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(- (+ 3 (- 4 5)) 6)");
    }
    {
        /// -+*&++---a
        /// - + * & ++ -- -a
        std::vector<Token> tokens = {
            { TOK_MINUS, "-" },
            { TOK_PLUS, "+" },
            { TOK_STAR, "*" },
            { TOK_AMPERSAND, "&" },
            { TOK_INC, "++" },
            { TOK_DEC, "--" },
            { TOK_MINUS, "-" },
            { TOK_ID, "a" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(- (+ (* (& (++ (-- (- a)))))))");
    }
    {
        /// -a+b
        /// -a + b
        std::vector<Token> tokens = {
            { TOK_MINUS, "-" },
            { TOK_ID, "a" },
            { TOK_PLUS, "+" },
            { TOK_ID, "b" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(+ (- a) b)");
    }
    {
        /// -+*&++---a+b
        /// (- + * & ++ -- -a)+b
        std::vector<Token> tokens = {
            { TOK_MINUS, "-" },
            { TOK_PLUS, "+" },
            { TOK_STAR, "*" },
            { TOK_AMPERSAND, "&" },
            { TOK_INC, "++" },
            { TOK_DEC, "--" },
            { TOK_MINUS, "-" },
            { TOK_ID, "a" },
            { TOK_PLUS, "+" },
            { TOK_ID, "b" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(+ (- (+ (* (& (++ (-- (- a))))))) b)");
    }
    {
        std::vector<Token> tokens = {
            { TOK_MINUS, "-" },
            { TOK_PLUS, "+" },
            { TOK_ID, "a" },
            { TOK_ASSIGN, "=" },
            { TOK_ID, "b" },
            { TOK_EQ, "==" },
            { TOK_NUM, "10" },
            { TOK_AND, "&&" },
            { TOK_ID, "c" },
            { TOK_GT, ">" },
            { TOK_NUM, "30" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(= (- (+ a)) (&& (== b 10) (> c 30)))");
    }
    {
        /// -+a = b == 10? c > 30 : d != 80
        /// (-(+a)) = ((b==10) ? (c>30) : (d!=80))
        std::vector<Token> tokens = {
            { TOK_MINUS, "-" },
            { TOK_PLUS, "+" },
            { TOK_ID, "a" },
            { TOK_ASSIGN, "=" },
            { TOK_ID, "b" },
            { TOK_EQ, "==" },
            { TOK_NUM, "10" },
            { TOK_QUESTION, "?" },
            { TOK_ID, "c" },
            { TOK_GT, ">" },
            { TOK_NUM, "30" },
            { TOK_COLON, ":" },
            { TOK_ID, "d" },
            { TOK_NE, "!=" },
            { TOK_NUM, "80" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(= (- (+ a)) (? (== b 10) (> c 30) (!= d 80)))");
    }

    {
        /// *++a++ = i==0 ? 2+3 : 4*5
        /// *(++ (a++)) = i==0 ? 2+3 : 4*5
        std::vector<Token> tokens = {
            { TOK_STAR, "*" },
            { TOK_INC, "++" },
            { TOK_ID, "a" },
            { TOK_INC, "++" },
            { TOK_ASSIGN, "=" },
            { TOK_ID, "i" },
            { TOK_EQ, "=="},
            { TOK_NUM, "0"},
            { TOK_QUESTION, "?"},
            { TOK_NUM, "2"},
            { TOK_PLUS, "+" },
            { TOK_NUM, "3" },
            { TOK_COLON, ":" },
            { TOK_NUM, "4"},
            { TOK_STAR, "*" },
            { TOK_NUM, "5" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(= (* (++ (a ++))) (? (== i 0) (+ 2 3) (* 4 5)))");
    }
    {
        /// -a[10][1]+b
        /// - (a[10][1]) + b
        std::vector<Token> tokens = {
            { TOK_MINUS, "-" },
            { TOK_ID, "a" },
            { TOK_LSQUAR, "[" },
            { TOK_NUM, "10" },
            { TOK_RSQUAR, "]" },
            { TOK_LSQUAR, "[" },
            { TOK_NUM, "1" },
            { TOK_RSQUAR, "]" },
            { TOK_PLUS, "+" },
            { TOK_ID, "b" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(+ (- ([ ([ a 10) 1)) b)");
    }
    {
        /// -a[10]--
        /// -(a[10]--)
        std::vector<Token> tokens = {
            { TOK_MINUS, "-" },
            { TOK_ID, "a" },
            { TOK_LSQUAR, "[" },
            { TOK_NUM, "10" },
            { TOK_RSQUAR, "]" },
            { TOK_DEC, "--" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(- (([ a 10) --))");
    }
    {
        /// a = b = c
        /// a = (b = c)
        std::vector<Token> tokens = {
            { TOK_ID, "a" },
            { TOK_ASSIGN, "=" },
            { TOK_ID, "b" },
            { TOK_ASSIGN, "=" },
            { TOK_ID, "c" },
        };

        Parser parser(tokens);
        auto ast = parser.parse();
        std::ostringstream ostr;
        ostr << *ast;
        std::string ast_str = ostr.str();
        std::cerr << "AST:\n" << ast_str << "\n";
        assert(ast_str == "(= a (= b c))");
    }
}
