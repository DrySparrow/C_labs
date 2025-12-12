#include <stdlib.h>

#include "bit_set.h"

struct bit_set {
    size_t capacity;
    int* data;
};

bit_set* bit_set_new(size_t bits) {
    const size_t bytes_needed = (bits + 7) / 8;
    bit_set* vector = malloc(sizeof(bit_set));
    if (vector == NULL)
        exit(0);
        
    vector->data = calloc(bytes_needed, sizeof(int));
    if (vector->data == NULL) {
        free(vector);
        return NULL;
    }
    
    vector->capacity = bits;
    return vector;
}

void bit_set_free(bit_set* vec) {
    if (vec) {
        free(vec->data);
        free(vec);
    }
}

int bit_set_get(bit_set* vec, size_t pos) {
    if (!vec || pos >= vec->capacity)
        return -1;
    return !!(vec->data[pos / 8] & (1U << (pos % 8)));
}

int bit_set_set(bit_set* vec, size_t pos) {
    if (!vec || pos >= vec->capacity)
        return -1;
    vec->data[pos / 8] |= (1U << (pos % 8));
    
    return 0;
}