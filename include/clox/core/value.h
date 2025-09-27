#ifndef CLOX_CORE_VALUE_H
#define CLOX_CORE_VALUE_H

#include <stddef.h>

/** @brief This typedef abstracts how Lox values are concretely represented in C
 */
typedef double Value;

void printValue(Value value);

#endif
