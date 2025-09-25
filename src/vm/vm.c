#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "clox/core/chunk.h"
#include "clox/core/value.h"
#include "clox/utils/debug.h"
#include "clox/utils/dynarr.h"
#include "clox/vm/dispatch.h"
#include "clox/vm/vm.h"
#include "config.h"

/*
 * The main bytecode execution loop
 * */
static InterpretResult executeBytecode(VM *vm) {
  for (;;) {

#ifdef DEBUG_TRACE_EXECUTION
    printf("Stack:");
    for (size_t i = 0; i < vm->stack.count; ++i) {
      Value slot = ((Value *)vm->stack.data)[i];
      printf(" [%g]", slot);
    }
    if (vm->stack.count > 0) {
      printf(" <- top");
    }
    printf("\n");

    size_t offset = (size_t)(vm->ip - (uint8_t *)vm->chunk->code.data);
    disassembleInstruction(vm->chunk, offset);
#endif

    uint8_t instruction = readInstruction(vm);
    switch (instruction) {
    case OP_CONSTANT: {
      Value constant = readConstant(vm);
      push(vm, constant);
      break;
    }
    case OP_ADD:
    case OP_SUBTRACT:
    case OP_MULTIPLY:
    case OP_DIVIDE:
      binaryOp(vm, instruction);
      break;
    case OP_NEGATE:
      push(vm, -pop(vm));
      break;
    case OP_RETURN: {
      // make sure there's something to pop
      if (vm->stack.count == 0) {
        fprintf(stderr, "Runtime error: stack underflow on OP_RETURN\n");
        return INTERPRET_RUNTIME_ERROR;
      }

      pop(vm);
      return INTERPRET_OK;
    }
    /* WARN: Is provisional */
    default:
      fprintf(stderr, "Runtime error: unknown opcode %d\n", instruction);
      return INTERPRET_RUNTIME_ERROR;
    }
  }
}

void initVM(VM *vm) {
  initDynArray(&vm->stack, sizeof(Value));
  vm->chunk = NULL;
  vm->ip = NULL;
}

void freeVM(VM *vm) { freeDynArray(&vm->stack); }

InterpretResult interpret(VM *vm, const char *source) {
  // vm->chunk = chunk;
  // Point to the beginning
  // vm->ip = (uint8_t *)chunk->code.data;
  // return executeBytecode(vm);
  compile(vm, source);
  return INTERPRET_OK;
}
