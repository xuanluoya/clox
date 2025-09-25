#ifndef CLOX_UTILS_DYNARR_H
#define CLOX_UTILS_DYNARR_H

#include <stddef.h>

typedef struct DynArray {
  size_t count;    // Current number of values in the dynamic array
  size_t capacity; // Allocated capacity
  size_t elemSize; // Dynamic array data type
  void *data;      // Dynamic array data
} DynArray;

void initDynArray(DynArray *array, size_t elemSize);
void pushDynArray(DynArray *array, void *element);
void freeDynArray(DynArray *array);

#endif
