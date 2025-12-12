#ifndef bitSet_h
#define bitSet_h

#include <stddef.h>

typedef struct bit_set bit_set;

bit_set* bit_set_new(size_t bits);
void bit_set_free(bit_set* vec);
int bit_set_get(bit_set* vec, size_t pos);
int bit_set_set(bit_set* vec, size_t pos);

#endif