#ifndef CLOX_VM_H
#define CLOX_VM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "clox/core/chunk.h"
#include "clox/core/value.h"
#include "clox/utils/dynarr.h"

typedef struct VM {
  Chunk *chunk;   ///< Currently loaded bytecode chunk
  uint8_t *ip;    ///< Instruction pointer into the chunk's code array
  DynArray stack; ///< Operand stack (Value)
} VM;

typedef enum InterpretResult {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
} InterpretResult;

/// @brief Read the next byte from the bytecode stream and advance the
/// instruction pointer
static inline uint8_t readInstruction(VM *vm) {
  /// return "*vm->ip" and ip++
  return *vm->ip++;
}

/// @brief Read Value from constants pool
static inline Value readConstant(VM *vm) {
  /// (Value *)vm->chunk->constants.data => Change the empty pointer to the
  /// Value pointer, and readInstruction(vm) will return ip
  return ((Value *)vm->chunk->constants.data)[readInstruction(vm)];
}

/// @brief Push a Value onto the top of the stack
static inline void push(VM *vm, Value value) {
  pushDynArray(&vm->stack, &value);
}

/// @brief Pop a Value from the top of the stack and return
/// @note 1. (Value *)vm->stack.data Change the empty pointer to the Value
/// - pointer
/// @note 2. + vm->stack.count Pointer arithmetic, move the position of count
/// - Value elements backward
/// @note 3. *(...) Dereference to obtain the Value stored at this location
static inline Value pop(VM *vm) {
  vm->stack.count--;

  return *((Value *)vm->stack.data + vm->stack.count);
}

/// @brief View elements of specified distance in the stack (no pop)
/// @note stack.count - 1 The position of the top element on the stack
/// @note stack.count - 1 - distance Target element position
static inline Value peek(VM *vm, size_t distance) {

  return *((Value *)vm->stack.data + vm->stack.count - 1 - distance);
}

void initVM(VM *vm);
void freeVM(VM *vm);
InterpretResult interpret(VM *vm, const char *source);

#endif
