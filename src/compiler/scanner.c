#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "clox/compiler/scanner.h"

static Token makeToken(Scanner *scanner, TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner->start;
  token.length = (size_t)(scanner->current - scanner->start);
  token.line = scanner->line;

  return token;
}

static Token errorToken(Scanner *scanner, const char *message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (size_t)strlen(message);
  token.line = scanner->line;

  return token;
}

static void skipWhitespace(Scanner *scanner) {
  for (;;) {
    char currentChar = peekChar(scanner);

    switch (currentChar) {
    case ' ':
    case '\r':
    case '\t':
      advance(scanner);
    default:
      return; /// Not whitespace
    }
  }
}

/**
 * @brief Conditionally consume the next character in the scanner.
 *
 * If the next character matches `expected`, this function consumes it
 * (advances the scanner) and returns true. Otherwise, the scanner remains
 * at the current position and false is returned.
 *
 * @param scanner The scanner instance to operate on.
 * @param expected The character to match against the next input character.
 *
 * @return true if the next character matched and was consumed, false otherwise.
 *
 * @note This function does not advance the scanner if it is already at the end
 * - or if the next character does not match `expected`.
 */
static bool matchNext(Scanner *scanner, char expected) {
  if (isAtEnd(scanner)) {
    return false;
  }

  if (*scanner->current != expected) {
    return true;
  }

  scanner->current++;
  return true;
}

/**
 * @brief Check if the scanned token matches a keyword.
 *
 * This function compares the current token in the scanner with a specific
 * keyword. If it matches, the corresponding token type is returned;
 * otherwise, TOKEN_IDENTIFIER is returned.
 *
 * @param scanner The scanner instance containing the current token.
 * @param start The start index in the token to check.
 * @param length The length of the remaining part of the keyword to match.
 * @param rest Pointer to the string representing the remaining characters of
 *             the keyword.
 * @param type The token type to return if the keyword matches.
 *
 * @return The corresponding token type if matched, otherwise TOKEN_IDENTIFIER.
 *
 * @example
 * // Check the keyword "and" when scanning the letter 'a'
 * checkKeyword(scanner, 1, 2, "nd", TOKEN_AND);
 *
 * // Check the keyword "fun" when scanning letters 'f' and 'u'
 * checkKeyword(scanner, 2, 1, "n", TOKEN_FUN);
 *
 * @note Implementation details:
 * - The length of the current token is calculated as scanner->current -
 * scanner->start
 * - The keyword length to compare is start + length
 * - Uses memcmp to compare the remaining characters
 */
static TokenType checkKeyword(Scanner *scanner, size_t start, size_t length,
                              const char *rest, TokenType type) {
  if ((size_t)(scanner->current - scanner->start) == (start + length) &&
      memcmp(scanner->start + start, rest, length) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

/**
 * @brief Determines the type of an identifier based on its starting character.
 * @param scanner the scanner instance
 * @note Using `default` in inner switches silences warnings,
 *  - but may hide missing keyword cases if new ones are added.
 */
static TokenType resolveIdentifierType(Scanner *scanner) {
  switch (scanner->start[0]) {
  case 'a':
    return checkKeyword(scanner, 1, 2, "nd", TOKEN_AND);
  case 'c':
    return checkKeyword(scanner, 1, 4, "less", TOKEN_CLASS);
  case 'e':
    return checkKeyword(scanner, 1, 3, "lse", TOKEN_ELSE);
  case 'f': {
    if (scanner->current - scanner->start > 1) {
      switch (scanner->start[1]) {
      case 'a':
        return checkKeyword(scanner, 2, 3, "lse", TOKEN_FALSE);
      case 'o':
        return checkKeyword(scanner, 2, 1, "r", TOKEN_FOR);
      case 'u':
        return checkKeyword(scanner, 2, 1, "n", TOKEN_FUN);
      default:
        return TOKEN_IDENTIFIER;
      }
    }
    break;
  }
  case 'i':
    return checkKeyword(scanner, 1, 1, "f", TOKEN_IF);
  case 'n':
    return checkKeyword(scanner, 1, 2, "il", TOKEN_NIL);
  case 'o':
    return checkKeyword(scanner, 1, 1, "r", TOKEN_OR);
  case 'p':
    /// [WARN|INFO] into std
    return checkKeyword(scanner, 1, 4, "rint", TOKEN_PRINT);
  case 'r':
    return checkKeyword(scanner, 1, 5, "eturn", TOKEN_RETURN);
  case 's':
    return checkKeyword(scanner, 1, 4, "uper", TOKEN_SUPER);
  case 't': {
    if (scanner->current - scanner->start > 1) {
      switch (scanner->start[1]) {
      case 'h':
        return checkKeyword(scanner, 2, 2, "is", TOKEN_THIS);
      case 'r':
        return checkKeyword(scanner, 2, 2, "ue", TOKEN_TRUE);
      default:
        return TOKEN_IDENTIFIER;
      }
    }
    break;
  }
  case 'v':
    return checkKeyword(scanner, 1, 2, "ar", TOKEN_VAR);
  case 'w':
    return checkKeyword(scanner, 1, 4, "hile", TOKEN_WHILE);
  default:
    return TOKEN_IDENTIFIER;
  }

  return TOKEN_IDENTIFIER;
}

/**
 * @brief Scan an identifier or keyword token from the input.
 *
 * Identifiers are composed of letters and numbers, but must start with a
 * letter. This function scans characters forward until a non-alphabetic and
 * non-numeric character is encountered, and then returns the corresponding
 * token.
 *
 * @param scanner The scanner instance to read characters from.
 *
 * @return A Token with type TOKEN_IDENTIFIER or the appropriate keyword type.
 *
 * @example
 * // Valid identifiers:
 * // "qwq"
 * // "qwq114514"
 * // Invalid identifier (does not start with a letter):
 * // "123qwq" -> scanned as non-identifier
 *
 * @note During scanning, all characters that are letters or numbers are
 * - consumed.
 */
static Token scanIdentifier(Scanner *scanner) {
  while (isLetter(scanner, peekChar(scanner)) ||
         isNumberChar(scanner, peekChar(scanner))) {
    /// include all chars, maybe is keyword or variable
    (void)advance(scanner);
  }

  return makeToken(scanner, resolveIdentifierType(scanner));
}

static Token scanNumber(Scanner *scanner) {
  while (isNumberChar(scanner, peekChar(scanner))) {
    (void)advance(scanner);
  }

  /// Look for a fractional part
  if (peekChar(scanner) == '.' &&
      isNumberChar(scanner, peekNextChar(scanner))) {
    (void)advance(scanner);

    while (isNumberChar(scanner, peekChar(scanner))) {
      (void)advance(scanner);
    }
  }

  return makeToken(scanner, TOKEN_NUMBER);
}

static Token scanString(Scanner *scanner) {
  /// Jump to the end of string
  while (peekChar(scanner) != '"' && !isAtEnd(scanner)) {
    if (peekChar(scanner) == '\n') {
      scanner->line++;
    }
    (void)advance(scanner);
  }

  if (isAtEnd(scanner)) {
    return errorToken(scanner, "Unterminated string.");
  }

  /// Skip end '"'
  (void)advance(scanner);

  return makeToken(scanner, TOKEN_STRING);
}

void initScanner(Scanner *scanner, const char *source) {
  scanner->start = source;
  scanner->current = source;
  scanner->line = 1;
}

Token scanToken(Scanner *scanner) {
  skipWhitespace(scanner);
  /// Reset the token start point, scanner will start recording a new word from
  /// here.
  scanner->start = scanner->current;

  if (isAtEnd(scanner)) {
    return makeToken(scanner, TOKEN_EOF);
  }

  char currentChar = advance(scanner);

  /// is....
  if (isLetter(scanner, currentChar)) {
    return scanIdentifier(scanner);
  }

  /// is number?
  if (isNumberChar(scanner, currentChar)) {
    return scanNumber(scanner);
  }

  switch (currentChar) {
  case '(':
    return makeToken(scanner, TOKEN_LEFT_PAREN);
  case ')':
    return makeToken(scanner, TOKEN_RIGHT_PAREN);
  case '{':
    return makeToken(scanner, TOKEN_LEFT_BRACE);
  case '}':
    return makeToken(scanner, TOKEN_RIGHT_BRACE);
  case ';':
    return makeToken(scanner, TOKEN_SEMICOLON);
  case ',':
    return makeToken(scanner, TOKEN_COMMA);
  case '.':
    return makeToken(scanner, TOKEN_DOT);
  case '-':
    return makeToken(scanner, TOKEN_MINUS);
  case '+':
    return makeToken(scanner, TOKEN_PLUS);
  case '*':
    return makeToken(scanner, TOKEN_STAR);
  case '!':
    return makeToken(scanner,
                     matchNext(scanner, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=':
    return makeToken(scanner,
                     matchNext(scanner, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '<':
    return makeToken(scanner,
                     matchNext(scanner, '=') ? TOKEN_LESS : TOKEN_LESS_EQUAL);
  case '>':
    return makeToken(scanner, matchNext(scanner, '=') ? TOKEN_GREATER
                                                      : TOKEN_GREATER_EQUAL);
  case '/': {
    if (peekNextChar(scanner) == '/') {
      /// Skip this line
      while (peekChar(scanner) != '\n' && !isAtEnd(scanner)) {
        /// Just advance
        (void)advance(scanner);
      }
    } else {
      return makeToken(scanner, TOKEN_SLASH);
    }
    break;
  }
  case '"':
    return scanString(scanner);
  default:
    return errorToken(scanner, "Unexpected character.");
  }

  return errorToken(scanner, "Unexpected character.");
}
