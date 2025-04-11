#pragma once

#include <vector>
#include <string>
#include <cstdint>

enum class TokenType {

};

struct Token {
    TokenType type;
    uint64_t val;
    size_t line;
};

class Lexer {
    public:
        explicit Lexer(const std::string& source);
        std::vector<Token> lex();

    private:
        std::string m_source;

        size_t m_current_pos;
        size_t m_current_line;
};