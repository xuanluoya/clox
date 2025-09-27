#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "clox/core/io.h"
#include "clox/utils/error.h"
#include "clox/vm/vm.h"

static char *readLine(void) {
  size_t capacity = INITIAL_LINE_CAPACITY;
  size_t length = 0;
  char *buffer = malloc(capacity);
  if (!buffer) {
    fatalError(ERR_FAILURE, "Malloc failed.");
  }

  int currentChar;
  while ((currentChar = getchar()) != EOF && currentChar != '\n') {
    if (length + 1 >= capacity) {
      capacity *= 2;
      char *newBuf = realloc(buffer, capacity);
      if (!newBuf) {
        free(buffer);
        fatalError(ERR_FAILURE, "Realloc failed.");
      }
      buffer = newBuf;
    }
    buffer[length++] = (char)currentChar;
  }

  if (currentChar == EOF && length == 0) {
    free(buffer);
    return NULL;
  }

  buffer[length] = '\0';

  /// Windows ENTER
  if (length > 0 && buffer[length - 1] == '\r') {
    buffer[length - 1] = '\0';
  }

  return buffer;
}

static char *readFile(const char *path) {
  /// "rb" => read binary
  /// Read binary do't care UTF-XX or ASCII even 1000101001010
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    fatalError(ERR_IO, "Cound not open file \"%s\".\n ", path);
  }

  /// Move file pointer to file end, is file size
  fseek(file, 0L, SEEK_END);
  /// Return pointer position
  long pos = ftell(file);
  if (pos < 0) {
    fclose(file);
    fatalError(ERR_IO, "ftell failed on \"%s\".\n", path);
  }
  size_t fileSize = (size_t)pos;
  /// Move pointer back to file begin
  if (fseek(file, 0L, SEEK_SET) != 0) {
    fclose(file);
    fatalError(ERR_IO, "I/O error: failed to rewind file \"%s\".\n", path);
  }

  /// Malloc buffer
  char *buffer = (char *)malloc(fileSize + 1);
  if (buffer == NULL) {
    fatalError(ERR_OS, "Not enough memory to read \"%s\".\n", path);
  }
  /// Read the content
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
    fatalError(ERR_IO, "Could not read file \"%s\".\n", path);
  }
  /// Add a terminator
  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}

void runREPL(VM *vm) {
  for (;;) {
    printf("> ");
    /// Force the buffer to be written to the terminal
    fflush(stdout);

    char *line = readLine();
    if (!line) {
      printf("\n");
      break;
    }

    /// interpret(vm, line);
    free(line);
  }
}

void executeFile(VM *vm, const char *path) {
  char *source = readFile(path);
  InterpretResult result = interpret(vm, source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR) {
    fatalError(ERR_COMPILE, "Compilation failed. See above for details.\n");
  }
  if (result == INTERPRET_RUNTIME_ERROR) {
    fatalError(ERR_RUNTIME, "Execution aborted due to a runtime error.\n");
  }
}
