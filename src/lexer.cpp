#include "lexer.h"

#include <cassert>
#include <cctype>

namespace {

[[nodiscard]] bool is_whitespace(char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

[[nodiscard]] bool is_identifier_start(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

[[nodiscard]] bool is_identifier_tail(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

[[nodiscard]] bool is_digit(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

[[nodiscard]] bool is_binary_digit(char c) { return c == '0' || c == '1'; }

[[nodiscard]] bool is_octal_digit(char c) { return c >= '0' && c <= '7'; }

[[nodiscard]] bool is_hex_digit(char c) {
    return std::isxdigit(static_cast<unsigned char>(c));
}

enum class NumberBase { Decimal, Binary, Octal, Hexadecimal };

[[nodiscard]] bool valid_digit(char c, NumberBase base) {
    // clang-format off
    switch (base) {
        case NumberBase::Decimal    : return is_digit(c);
        case NumberBase::Binary     : return is_binary_digit(c);
        case NumberBase::Octal      : return is_octal_digit(c);
        case NumberBase::Hexadecimal: return is_hex_digit(c);
    }
    // clang-format on

    return false;
}

[[nodiscard]] std::string base_name(NumberBase base) {
    // clang-format off
    switch (base) {
        case NumberBase::Decimal    : return "decimal";
        case NumberBase::Binary     : return "binary";
        case NumberBase::Octal      : return "octal";
        case NumberBase::Hexadecimal: return "hexadecimal";
    }
    // clang-format on

    return "unknown";
}

} // namespace

const std::unordered_map<std::string, TokenType> Lexer::_keywords{
    {"spec", TokenType::Spec},
    {"input", TokenType::Input},
    {"state", TokenType::State},
    {"output", TokenType::Output},
    {"label", TokenType::Label},
    {"behavior", TokenType::Behavior},
    {"next", TokenType::Next},
    {"forall", TokenType::Forall},
    {"in", TokenType::In},
    {"if", TokenType::If},
    {"then", TokenType::Then},
    {"else", TokenType::Else},
    {"switch", TokenType::Switch},
    {"case", TokenType::Case},
    {"default", TokenType::Default},
    {"S", TokenType::Signal},
    {"B", TokenType::Bool},
};

const std::unordered_map<char, TokenType> Lexer::_single_char_tokens{
    {'-', TokenType::Minus},
    {'|', TokenType::Pipe},
    {'~', TokenType::Tilde},
    {'?', TokenType::Question},
    {'{', TokenType::LBrace},
    {'}', TokenType::RBrace},
    {'[', TokenType::LBracket},
    {']', TokenType::RBracket},
    {'(', TokenType::LParen},
    {')', TokenType::RParen},
    {':', TokenType::Colon},
};

Lexer::Lexer(std::string src) : _src(std::move(src)) {}

std::vector<Token> Lexer::lex() {
    std::vector<Token> tokens;

    while (!eof()) {
        if (is_whitespace(current())) {
            consume();
            continue;
        }

        std::size_t start = _pos;
        std::size_t line = _line;
        std::size_t column = _column;

        if (is_identifier_start(current())) {
            lex_identifier(tokens, start, line, column);
        }
        else if (is_digit(current())) {
            lex_number(tokens, start, line, column);
        }
        else if (current() == '=') {
            lex_equals(tokens, start, line, column);
        }
        else if (current() == '!') {
            lex_not_equal(tokens, start, line, column);
        }
        else if (current() == '.') {
            lex_ellipsis(tokens, start, line, column);
        }
        else if (lex_single_char_token(tokens, start, line, column)) {
            continue;
        }
        else {
            std::string msg =
                "unexpected character '" + std::string(1, current()) + "'";
            throw LexerError(line, column, std::move(msg));
        }
    }

    return tokens;
}

bool Lexer::eof() const { return _pos >= _src.size(); }

std::optional<char> Lexer::at(std::size_t pos) const {
    if (pos >= _src.size()) return std::nullopt;
    return _src[pos];
}

std::optional<char> Lexer::peek(std::size_t ahead) const {
    return at(_pos + ahead);
}

char Lexer::current() const {
    assert(!eof());
    return _src[_pos];
}

char Lexer::consume() {
    assert(!eof());

    char c = _src[_pos++];

    if (c == '\n') {
        ++_line;
        _column = 1;
    }
    else {
        ++_column;
    }

    return c;
}

void Lexer::emit(
    std::vector<Token>& tokens,
    TokenType type,
    std::size_t start,
    std::size_t line,
    std::size_t column
) {
    tokens.push_back({type, _src.substr(start, _pos - start), line, column});
}

void Lexer::lex_identifier(
    std::vector<Token>& tokens,
    std::size_t start,
    std::size_t line,
    std::size_t column
) {
    consume();

    while (!eof() && is_identifier_tail(current())) {
        consume();
    }

    std::string value = _src.substr(start, _pos - start);

    auto it = _keywords.find(value);
    TokenType type = it != _keywords.end() ? it->second : TokenType::Identifier;

    tokens.push_back({type, std::move(value), line, column});
}

void Lexer::lex_number(
    std::vector<Token>& tokens,
    std::size_t start,
    std::size_t line,
    std::size_t column
) {
    NumberBase base = NumberBase::Decimal;

    if (current() == '0') {
        if (peek(1) == 'b') {
            base = NumberBase::Binary;
            consume();
            consume();
        }
        else if (peek(1) == 'o') {
            base = NumberBase::Octal;
            consume();
            consume();
        }
        else if (peek(1) == 'x' || peek(1) == 'X') {
            base = NumberBase::Hexadecimal;
            consume();
            consume();
        }
    }

    if (eof() || is_whitespace(current())) {
        std::string msg = "expected " + std::string(base_name(base)) + " digit";
        throw LexerError(_line, _column, std::move(msg));
    }

    if (!valid_digit(current(), base)) {
        std::string msg = "invalid " + std::string(base_name(base)) + " digit '"
                          + std::string(1, current()) + "'";
        throw LexerError(_line, _column, std::move(msg));
    }

    while (!eof() && valid_digit(current(), base)) {
        consume();
    }

    if (!eof() && is_identifier_tail(current())) {
        std::string msg = "invalid character '" + std::string(1, current())
                          + "' in numeric literal";
        throw LexerError(_line, _column, std::move(msg));
    }

    std::string value = _src.substr(start, _pos - start);

    tokens.push_back({TokenType::Number, std::move(value), line, column});
}

void Lexer::lex_equals(
    std::vector<Token>& tokens,
    std::size_t start,
    std::size_t line,
    std::size_t column
) {
    consume();

    if (!eof() && current() == '=') {
        consume();
        emit(tokens, TokenType::EqualEqual, start, line, column);
    }
    else {
        emit(tokens, TokenType::Equal, start, line, column);
    }
}

void Lexer::lex_not_equal(
    std::vector<Token>& tokens,
    std::size_t start,
    std::size_t line,
    std::size_t column
) {
    consume();

    if (!eof() && current() == '=') {
        consume();
        emit(tokens, TokenType::NotEqual, start, line, column);
    }
    else {
        throw LexerError(line, column, "expected '=' after '!'");
    }
}

void Lexer::lex_ellipsis(
    std::vector<Token>& tokens,
    std::size_t start,
    std::size_t line,
    std::size_t column
) {
    if (peek(1) == '.' && peek(2) == '.') {
        consume();
        consume();
        consume();

        emit(tokens, TokenType::Ellipsis, start, line, column);
    }
    else {
        throw LexerError(line, column, "expected '...'");
    }
}

bool Lexer::lex_single_char_token(
    std::vector<Token>& tokens,
    std::size_t start,
    std::size_t line,
    std::size_t column
) {
    auto it = _single_char_tokens.find(current());

    if (it != _single_char_tokens.end()) {
        consume();
        emit(tokens, it->second, start, line, column);
        return true;
    }

    return false;
}