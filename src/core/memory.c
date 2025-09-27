#include <stdio.h>
#include <stdlib.h>

#include "clox/core/memory.h"
#include "clox/utils/error.h"

/*
 * @note This reallocate() function is the single function
 * - we’ll use for all dynamic memory management in clox—allocating memory,
 * - note freeing it, and changing the size of an existing allocation.
 */
void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void *result = realloc(pointer, newSize);
  if (result == NULL) {
    fatalError(ERR_FAILURE, "An error occurred when reallocating memory");
  }
  return result;
}
