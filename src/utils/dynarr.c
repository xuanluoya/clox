#include <stddef.h>
#include <string.h>

#include "clox/core/memory.h"
#include "clox/utils/dynarr.h"

void initDynArray(DynArray *array, size_t elemSize) {
  array->count = 0;
  array->capacity = 0;
  array->elemSize = elemSize;
  array->data = NULL;
}

void pushDynArray(DynArray *array, void *element) {
  if (array->capacity < array->count + 1) {
    size_t oldSize = array->capacity;
    array->capacity = grow_capacity(oldSize);
    array->data = grow_array(array->data, oldSize, array->capacity,
                             sizeof(array->elemSize));
  }

  memcpy((char *)array->data + (array->count * array->elemSize), element,
         array->elemSize);
  // auto add one
  array->count++;
}

void freeDynArray(DynArray *array) {
  array->data = free_array(array->data, array->capacity, array->elemSize);
  initDynArray(array, array->elemSize);
}
