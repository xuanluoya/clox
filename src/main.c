#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "clox/core/io.h"
#include "clox/utils/error.h"
#include "clox/vm/vm.h"

int main(int argc, char *argv[]) {
  VM vm;
  initVM(&vm);

  if (argc == 1) {
    repl(&vm);
  } else if (argc == 2) {
    runFile(&vm, argv[1]);
  } else {
    fatalError(ERR_USAGE, "Usage: clox [path]\n");
  }
  freeVM(&vm);

  return EXIT_SUCCESS;
}
