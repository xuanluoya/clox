#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "clox/core/chunk.h"
#include "clox/core/value.h"
#include "clox/utils/debug.h"

static size_t constantInstruction(const char *name, Chunk *chunk,
                                  size_t offset) {
  uint8_t *codes = (uint8_t *)chunk->code.data;
  Value *values = (Value *)chunk->constants.data;
  uint8_t constant_index = codes[offset + 1];

  printf("%-16s %4d '", name, constant_index);
  printValue(values[constant_index]);
  printf("'\n");
  return offset + 2;
}

static size_t simpleInstruction(const char *name, size_t offset) {
  printf("%s\n", name);
  return offset + 1;
}

size_t disassembleInstruction(Chunk *chunk, size_t offset) {
  printf("%04zu %04zu ", offset, getLine(chunk, offset));

  uint8_t *codes = (uint8_t *)chunk->code.data;
  uint8_t instruction = codes[offset];

  switch (instruction) {
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", chunk, offset);
  case OP_ADD:
    return simpleInstruction("OP_ADD", offset);
  case OP_SUBTRACT:
    return simpleInstruction("OP_SUBTRACT", offset);
  case OP_MULTIPLY:
    return simpleInstruction("OP_MULTIPLY", offset);
  case OP_DIVIDE:
    return simpleInstruction("OP_DIVIDE", offset);
  case OP_NEGATE:
    return simpleInstruction("OP_NEGATE", offset);
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  default:
    printf("%-16s %4s %s\n", "UNKNOWN", "-", "opcode");
    printf("     (raw byte = %d)\n", instruction);
    return offset + 1;
  }
}

void disassembleChunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);
  printf("%-4s %-4s %-16s %-4s %s\n", "Code", "Line", "OpCode", "Slot",
         "Value");

  for (size_t offset = 0; offset < chunk->code.count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}
