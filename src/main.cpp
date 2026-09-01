#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "lexer.h"

std::string token_type_to_str(TokenType type) {
    switch (type) {
    case TokenType::Identifier:
        return "Identifier";
    case TokenType::Number:
        return "Number";

    case TokenType::Spec:
        return "Spec";
    case TokenType::Input:
        return "Input";
    case TokenType::State:
        return "State";
    case TokenType::Output:
        return "Output";
    case TokenType::Label:
        return "Label";
    case TokenType::Behavior:
        return "Behavior";
    case TokenType::Next:
        return "Next";
    case TokenType::Forall:
        return "Forall";
    case TokenType::In:
        return "In";
    case TokenType::If:
        return "If";
    case TokenType::Then:
        return "Then";
    case TokenType::Else:
        return "Else";
    case TokenType::Switch:
        return "Switch";
    case TokenType::Case:
        return "Case";
    case TokenType::Default:
        return "Default";

    case TokenType::Signal:
        return "Signal";
    case TokenType::Bool:
        return "Bool";

    case TokenType::Equal:
        return "Equal";
    case TokenType::EqualEqual:
        return "EqualEqual";
    case TokenType::NotEqual:
        return "NotEqual";
    case TokenType::Tilde:
        return "Tilde";
    case TokenType::Pipe:
        return "Pipe";
    case TokenType::Minus:
        return "Minus";
    case TokenType::Question:
        return "Question";

    case TokenType::LBrace:
        return "LBrace";
    case TokenType::RBrace:
        return "RBrace";
    case TokenType::LBracket:
        return "LBracket";
    case TokenType::RBracket:
        return "RBracket";
    case TokenType::LParen:
        return "LParen";
    case TokenType::RParen:
        return "RParen";
    case TokenType::Colon:
        return "Colon";
    case TokenType::Ellipsis:
        return "Ellipsis";

    case TokenType::EndOfFile:
        return "EndOfFile";
    }

    return "Unknown";
}

void print_token(const Token& token) {
    std::cout << token_type_to_str(token.type) << " \"" << token.value << "\""
              << " at " << token.line << ":" << token.column << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << argv[0] << ": error: no input files" << std::endl;
        return EXIT_FAILURE;
    }

    std::string source;
    {
        std::stringstream source_stream;
        std::ifstream input{argv[1]};
        source_stream << input.rdbuf();
        source = source_stream.str();
    }

    std::cout << source << std::endl;

    Lexer lexer(std::move(source));
    std::vector<Token> tokens;

    try {
        tokens = lexer.lex();
    }
    catch (LexerError e) {
        std::cerr << argv[1] << ':' << e.line() << ':' << e.column()
                  << ": error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << std::endl;

    for (const Token& t : tokens) {
        print_token(t);
    }

    return EXIT_SUCCESS;
}