#ifndef CLOX_CORE_MEMORY_H
#define CLOX_CORE_MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void *reallocate(void *pointer, size_t oldSize, size_t newSize);

/* Expand when the capacity is full */
static inline size_t grow_capacity(size_t old) { return old < 8 ? 8 : old * 2; }

/* Reallocate more memory */
static inline void *grow_array(void *pointer, size_t oldCount, size_t newCount,
                               size_t elemSize) {
  size_t oldSize = oldCount * elemSize;
  size_t newSize = newCount * elemSize;
  void *result = reallocate(pointer, oldSize, newSize);
  if (result == NULL) {
    fprintf(stderr, "Memory reallocation failed (old=%zu, new=%zu, elem=%zu)\n",
            oldCount, newCount, elemSize);
    exit(EXIT_FAILURE);
  }
  return result;
}

/* Free array */
static inline void *free_array(void *pointer, size_t oldCont,
                               size_t elementSize) {
  size_t oldSize = oldCont * elementSize;
  return reallocate(pointer, oldSize, 0);
}

#endif
