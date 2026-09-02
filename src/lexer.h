#pragma once

#include <cstdlib>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

enum class TokenType {
    // Literals
    Identifier,
    Number,

    // Keywords
    Spec,
    Input,
    State,
    Output,
    Label,
    Behavior,
    Next,
    Forall,
    In,
    If,
    Then,
    Else,
    Switch,
    Case,
    Default,

    // Types
    Signal,
    Bool,

    // Operators
    Equal,      // =
    EqualEqual, // ==
    NotEqual,   // !=
    Tilde,      // ~
    Pipe,       // |
    Minus,      // -
    Question,   // ?

    // Delimiters
    LBrace,   // {
    RBrace,   // }
    LBracket, // [
    RBracket, // ]
    LParen,   // (
    RParen,   // )
    Colon,    // :
    Ellipsis, // ...
    Newline,  // \n

    EndOfFile
};

struct Token {
    TokenType type;
    std::string value;
    std::size_t line;
    std::size_t column;
};

class Lexer {
  public:
    explicit Lexer(std::string src);

    std::vector<Token> lex();

  private:
    std::string _src;

    std::size_t _pos = 0;
    std::size_t _line = 1;
    std::size_t _column = 1;

    [[nodiscard]] bool eof() const;

    [[nodiscard]] std::optional<char> at(std::size_t pos) const;
    [[nodiscard]] std::optional<char> peek(std::size_t ahead = 1) const;

    char current() const;
    char consume();

    void emit(
        std::vector<Token>& tokens,
        TokenType type,
        std::size_t start,
        std::size_t line,
        std::size_t column
    );

    void lex_newline(std::vector<Token>& tokens);

    void lex_identifier(
        std::vector<Token>& tokens,
        std::size_t start,
        std::size_t line,
        std::size_t column
    );

    void lex_number(
        std::vector<Token>& tokens,
        std::size_t start,
        std::size_t line,
        std::size_t column
    );

    void lex_equals(
        std::vector<Token>& tokens,
        std::size_t start,
        std::size_t line,
        std::size_t column
    );

    void lex_not_equal(
        std::vector<Token>& tokens,
        std::size_t start,
        std::size_t line,
        std::size_t column
    );

    void lex_ellipsis(
        std::vector<Token>& tokens,
        std::size_t start,
        std::size_t line,
        std::size_t column
    );

    [[nodiscard]] bool lex_single_char_token(
        std::vector<Token>& tokens,
        std::size_t start,
        std::size_t line,
        std::size_t column
    );

    static const std::unordered_map<std::string, TokenType> _keywords;
    static const std::unordered_map<char, TokenType> _single_char_tokens;
};

class LexerError : public std::runtime_error {
  public:
    LexerError(std::size_t line, std::size_t column, std::string message)
        : std::runtime_error(std::move(message)), _line(line), _column(column) {
    }

    [[nodiscard]] std::size_t line() const { return _line; }
    [[nodiscard]] std::size_t column() const { return _column; }

  private:
    std::size_t _line;
    std::size_t _column;
};