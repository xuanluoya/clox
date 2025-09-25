#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "clox/utils/error.h"

void fatalError(ErrorCode code, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "Fatal: ");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit((int)code);
}
