#ifndef CLOX_COMPILER_SCANNER
#define CLOX_COMPILER_SCANNER

#include <stdbool.h>
#include <stddef.h>

/**
 * @enum TokenType
 * @brief Represents all possible token types in the language.
 *
 * Tokens are classified as single-character symbols, multi-character symbols,
 * literals, keywords, and special tokens.
 */
typedef enum TokenType {
  // Single-character tokens
  TOKEN_LEFT_PAREN,  ///< '('
  TOKEN_RIGHT_PAREN, ///< ')'
  TOKEN_LEFT_BRACE,  ///< '{'
  TOKEN_RIGHT_BRACE, ///< '}'
  TOKEN_COMMA,       ///< ','
  TOKEN_DOT,         ///< '.'
  TOKEN_MINUS,       ///< '-'
  TOKEN_PLUS,        ///< '+'
  TOKEN_SEMICOLON,   ///< ';'
  TOKEN_SLASH,       ///< '/'
  TOKEN_STAR,        ///< '*'

  // One or two character tokens
  TOKEN_BANG,          ///< '!'
  TOKEN_BANG_EQUAL,    ///< '!='
  TOKEN_EQUAL,         ///< '='
  TOKEN_EQUAL_EQUAL,   ///< '=='
  TOKEN_GREATER,       ///< '>'
  TOKEN_GREATER_EQUAL, ///< '>='
  TOKEN_LESS,          ///< '<'
  TOKEN_LESS_EQUAL,    ///< '<='

  // Literals
  TOKEN_IDENTIFIER, ///< identifier
  TOKEN_STRING,     ///< string literal
  TOKEN_NUMBER,     ///< number literal

  // Keywords
  TOKEN_AND,    ///< and
  TOKEN_CLASS,  ///< class
  TOKEN_ELSE,   ///< else
  TOKEN_FALSE,  ///< false
  TOKEN_FOR,    ///< for
  TOKEN_FUN,    ///< fun
  TOKEN_IF,     ///< if
  TOKEN_NIL,    ///< nil
  TOKEN_OR,     ///< or
  TOKEN_PRINT,  ///< print
  TOKEN_RETURN, ///< return
  TOKEN_SUPER,  ///< super
  TOKEN_THIS,   ///< this
  TOKEN_TRUE,   ///< true
  TOKEN_VAR,    ///< var
  TOKEN_WHILE,  ///< while

  // Special tokens
  TOKEN_ERROR, ///< error placeholder
  TOKEN_EOF    ///< end of file
} TokenType;

/**
 * @brief Represents the smallest syntactic unit (token) in source code.
 *
 * @details When the scanner scans the source code, it generates a Token
 * instance each time it encounters a token.
 *
 * @var Token::type
 * The token type.
 *
 * @var Token::start
 * A pointer to the starting position of a token in the source code.
 * This does **not** copy the string.
 *
 * @var Token::length
 * The length (in characters) of the token, used together with `start` to obtain
 * the token text.
 *
 * @var Token::line
 * The line number where the token appears, used for error prompts and
 * debugging.
 *
 * @example
 * // Example: source line 1: "var x = 10"
 * // Token for "var"
 * //  - type   = TOKEN_VAR
 * //  - start  = 'v'
 * //  - length = 3
 * //  - line   = 1
 */

typedef struct Token {
  TokenType type;    ///< Token type
  const char *start; ///< Point to the token starting location
  size_t length;     ///< Token length
  size_t line;       ///< The line number where the token is located
} Token;

typedef struct Sanner {
  const char *start;   ///< Pointer to token start char
  const char *current; ///< The character pointing to the present
  size_t line;         ///< The line number being scanned now
} Scanner;

static inline char advance(Scanner *scanner) { return *scanner->current++; }
static inline char peekChar(Scanner *scanner) { return *scanner->current; }

static inline bool isAtEnd(Scanner *scanner) {
  return *(scanner->current) == '\0';
}

static inline char peekNextChar(Scanner *scanner) {
  if (isAtEnd(scanner)) {
    return '\0';
  }
  return scanner->current[1];
}

static inline bool isNumberChar(Scanner *scanner, char thisChar) {
  return thisChar >= '0' && thisChar <= '9';
}

static inline bool isLetter(Scanner *scanner, char thisChar) {
  return (thisChar >= 'a' && thisChar <= 'z') ||
         (thisChar >= 'A' && thisChar <= 'Z') || thisChar == '_';
}

void initScanner(Scanner *scanner, const char *source);
Token scanToken(Scanner *scanner);

#endif
