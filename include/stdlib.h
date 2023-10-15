/*
File:   stdlib.h
Author: Taylor Robbins
Date:   10\07\2023
*/

#ifndef _STDLIB_H
#define _STDLIB_H

#include <internal/std_common.h>
#include "stdint.h"

CONDITIONAL_EXTERN_C_START

void* malloc(size_t numBytes);
void* calloc(size_t numElements, size_t elemSize);
void* realloc(void* prevAllocPntr, size_t newSize);
void free(void* allocPntr);
void* aligned_alloc(size_t numBytes, size_t alignmentSize);

#define RAND_MAX (0x7FFFFFFF)
void srand(unsigned int seed);
int rand();

double atof(const char* str);
void* alloca(size_t numBytes);

CONDITIONAL_EXTERN_C_END

#endif //  _STDLIB_H

// +--------------------------------------------------------------+
// |                   Autocomplete Dictionary                    |
// +--------------------------------------------------------------+
/*
@Defines
RAND_MAX
@Functions
void* malloc(size_t numBytes)
void* calloc(size_t numElements, size_t elemSize)
void* realloc(void* prevAllocPntr, size_t newSize)
void free(void* allocPntr)
void* aligned_alloc(size_t numBytes, size_t alignmentSize)
void srand(unsigned int seed)
int rand()
double atof(const char* str)
void* alloca(size_t numBytes)
*/
