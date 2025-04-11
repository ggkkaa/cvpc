#include "lexer.hpp"

Lexer::Lexer(const std::string &source)
    : m_source(source), m_current_pos(0), m_current_line(0) {}