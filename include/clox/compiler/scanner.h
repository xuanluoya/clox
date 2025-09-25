#ifndef CLOX_COMPILER_SCANNER
#define CLOX_COMPILER_SCANNER

typedef struct Sanner {
  const char *start;
  const char *current;
} Scanner;

void initScanner(const char *source);

#endif
