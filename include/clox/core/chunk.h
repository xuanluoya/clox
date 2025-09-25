/* equences of bytecode */
#ifndef CLOX_CORE_CHUNK_H
#define CLOX_CORE_CHUNK_H

#include <stddef.h>
#include <stdint.h>

#include "clox/core/value.h"
#include "clox/utils/dynarr.h"

/* Bytecode operation codes (instructions for the VM) */
typedef enum OpCode {
  OP_CONSTANT, // Push a constant value onto the stack
  OP_ADD,      // Add the top two stack values (a + b)
  OP_SUBTRACT, // Subtract the top two stack values (a - b)
  OP_MULTIPLY, // Multiply the top two stack values (a * b)
  OP_DIVIDE,   // Divide the top two stack values (a / b)
  OP_NEGATE,   // Negate the top stack value (-a)
  OP_RETURN,   // Return from the current function
} OpCode;

/* A run-length record for source line information.
 * Represents a source code line and how many consecutiveinstructions
 * instructions were generated from that line. */
typedef struct LineRecord {
  size_t line;  // Source code line number
  size_t count; // Number of consecutive instructions from this line
} LineRecord;

/*
 * Represents a contiguous sequence of bytecode instructions
 * A Chunk is the basic unit of executable code in Lox
 * */
typedef struct Chunk {
  DynArray code;      // Dynamic array of bytecode instructions (uint8_t)
  DynArray constants; // Constants pool
  DynArray lines;     // Source code line info
} Chunk;

void initChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, size_t line);
size_t addConstant(Chunk *chunk, Value value);
void freeChunk(Chunk *chunk);
size_t getLine(const Chunk *chunk, size_t instructionsIndex);

#endif
