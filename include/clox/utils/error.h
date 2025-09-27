#ifndef CLOX_UTILS_ERROR_H
#define CLOX_UTILS_ERROR_H

#include <stdlib.h>

typedef enum ErrorCode {
  ERR_OK = EXIT_SUCCESS,
  ERR_FAILURE = EXIT_FAILURE,

  ERR_USAGE = 64,   ///< EX_USAGE
  ERR_DATAERR = 65, ///< EX_DATAERR
  ERR_COMPILE = 65, ///< alias
  ERR_RUNTIME = 70, ///< EX_SOFTWARE
  ERR_OS = 71,      ///< EX_OSERR
  ERR_IO = 74,      ///< EX_IOERR
} ErrorCode;

__attribute__((format(printf, 2, 3))) void fatalError(ErrorCode code,
                                                      const char *fmt, ...);

#endif
