#ifndef CLOX_VM_DISPATCH_H
#define CLOX_VM_DISPATCH_H

#include "clox/core/chunk.h"
#include "clox/vm/vm.h"

// Function Pointers of binary operators
// [WARN|TODO] : In future, Value and Value can use binary arithmetic too
typedef void (*BinaryOpFunc)(VM *, double, double);

static inline void addImpl(VM *vm, double a, double b) { push(vm, a + b); }
static inline void subtractImpl(VM *vm, double a, double b) { push(vm, a - b); }
static inline void multiplyImpl(VM *vm, double a, double b) { push(vm, a * b); }
static inline void divideImpl(VM *vm, double a, double b) { push(vm, a / b); }

static const BinaryOpFunc binaryOps[5] = {
    [OP_ADD] = addImpl,
    [OP_SUBTRACT] = subtractImpl,
    [OP_MULTIPLY] = multiplyImpl,
    [OP_DIVIDE] = divideImpl,
};

static inline void binaryOp(VM *vm, OpCode op) {
  if (op < OP_ADD || op > OP_DIVIDE || !binaryOps[op]) {
    // [WARN|TODO]
    return;
  }

  double b = pop(vm);
  double a = pop(vm);
  binaryOps[op](vm, a, b);
}

#endif
