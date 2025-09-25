#ifndef CLOX_UTILS_DEBUG_H
#define CLOX_UTILS_DEBUG_H

#include <stddef.h>

#include "clox/core/chunk.h"

size_t disassembleInstruction(Chunk *chunk, size_t offset);
void disassembleChunk(Chunk *chunk, const char *name);

#endif
