#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_CHARS 2048
#define MAX_CODE_LEN 2048
#define BUFFER_SIZE 8192

typedef struct Node {
    char data;
    int freq;
    struct Node *left, *right;
} Node, *pointer_huffman;

typedef struct {
    unsigned char code_bytes[MAX_CODE_LEN/8 + 1];
    int code_len_bits;
} HuffmanCodeEntry;

typedef struct {
    pointer_huffman root;
    HuffmanCodeEntry huffman_codes[MAX_CHARS];
    pointer_huffman* min_heap_nodes;
    int min_heap_size;
    int min_heap_capacity;
} HuffmanContext;

typedef struct {
    unsigned char* buffer;
    size_t capacity;
    size_t data_len;
    size_t current_byte_idx;
    int current_bit_offset;
} BitBuffer;

// Прототипы функций
void huffman_context_init(HuffmanContext* ctx);
void huffman_context_destroy(HuffmanContext* ctx);
unsigned char* huffman_encode_to_memory(HuffmanContext* ctx, const char* input_text, size_t* compressed_size, size_t len_data);
char* huffman_decode_from_memory(HuffmanContext* ctx, const unsigned char* compressed_data, size_t compressed_size, size_t* decoded_len);

#endif