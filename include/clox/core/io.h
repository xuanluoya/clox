#ifndef CLOX_CORE_IO_H
#define CLOX_CORE_IO_H

#include "clox/vm/vm.h"

#define INITIAL_LINE_CAPACITY 1024

void runREPL(VM *vm);
void executeFile(VM *vm, const char *path);

#endif
