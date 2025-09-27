#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "clox/core/chunk.h"
#include "clox/core/value.h"
#include "clox/utils/dynarr.h"

void initChunk(Chunk *chunk) {
  initDynArray(&chunk->code, sizeof(uint8_t));
  initDynArray(&chunk->lines, sizeof(LineRecord));
  initDynArray(&chunk->constants, sizeof(Value));
}

void writeChunk(Chunk *chunk, uint8_t byte, size_t line) {
  pushDynArray(&chunk->code, &byte);

  LineRecord *lines = (LineRecord *)chunk->lines.data;

  if (chunk->lines.count > 0 && lines[chunk->lines.count - 1].line == line) {
    lines[chunk->lines.count - 1].count++;
  } else {
    LineRecord rec = {.line = line, .count = 1};
    pushDynArray(&chunk->lines, &rec);
  }
}

/// @brief Return the index of constants in the constant pool
size_t addConstant(Chunk *chunk, Value value) {
  pushDynArray(&chunk->constants, &value);
  return chunk->constants.count - 1;
}

void freeChunk(Chunk *chunk) {
  freeDynArray(&chunk->code);
  freeDynArray(&chunk->constants);
  freeDynArray(&chunk->lines);
}

size_t getLine(const Chunk *chunk, size_t instructionsIndex) {
  LineRecord *lines = (LineRecord *)chunk->lines.data;
  size_t offset = 0;

  for (size_t i = 0; i < chunk->lines.count; ++i) {
    offset += lines[i].count;
    if (instructionsIndex < offset) {
      return lines[i].line;
    }
  }

  return 0;
}
