#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

pointer_huffman create_huffman_node(char data, int freq) {
    pointer_huffman node = (pointer_huffman)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_huffman_tree(pointer_huffman root) {
    if (root == NULL) {
        return;
    }
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}

void huffman_context_init(HuffmanContext* ctx) {
    ctx->root = NULL;
    for (int i = 0; i < MAX_CHARS; ++i) {
        ctx->huffman_codes[i].code_len_bits = 0;
        memset(ctx->huffman_codes[i].code_bytes, 0, sizeof(ctx->huffman_codes[i].code_bytes));
    }
    ctx->min_heap_capacity = MAX_CHARS;
    ctx->min_heap_nodes = (pointer_huffman*)malloc(ctx->min_heap_capacity * sizeof(pointer_huffman));
    if (ctx->min_heap_nodes == NULL) {
        exit(EXIT_FAILURE);
    }
    ctx->min_heap_size = 0;
}

void huffman_context_destroy(HuffmanContext* ctx) {
    free_huffman_tree(ctx->root);
    ctx->root = NULL;

    if (ctx->min_heap_nodes != NULL) {
        free(ctx->min_heap_nodes);
        ctx->min_heap_nodes = NULL;
    }
    ctx->min_heap_size = 0;
    ctx->min_heap_capacity = 0;
}

int compare_nodes(const void* a, const void* b) {
    Node* nodeA = *(Node**)a;
    Node* nodeB = *(Node**)b;
    return nodeA->freq - nodeB->freq;
}

void generate_huffman_codes(HuffmanContext* ctx, pointer_huffman node, unsigned char* current_code_bytes, int current_code_len_bits) {
    if (current_code_len_bits >= MAX_CODE_LEN) {
        exit(EXIT_FAILURE);
    }

    if (node->left == NULL && node->right == NULL) {
        HuffmanCodeEntry* entry = &ctx->huffman_codes[(unsigned char)node->data];
        entry->code_len_bits = current_code_len_bits;
        size_t bytes_to_copy = (current_code_len_bits + 7) / 8;
        memcpy(entry->code_bytes, current_code_bytes, bytes_to_copy);
        if (current_code_len_bits % 8 != 0) {
            unsigned char mask = (1 << (8 - (current_code_len_bits % 8))) - 1;
            entry->code_bytes[bytes_to_copy - 1] &= ~mask;
        }
        return;
    }

    if (node->left != NULL) {
        int byte_idx = current_code_len_bits / 8;
        int bit_offset = current_code_len_bits % 8;
        unsigned char original_byte_value = current_code_bytes[byte_idx];

        current_code_bytes[byte_idx] &= ~(1 << (7 - bit_offset));
        generate_huffman_codes(ctx, node->left, current_code_bytes, current_code_len_bits + 1);
        current_code_bytes[byte_idx] = original_byte_value;
    }

    if (node->right != NULL) {
        int byte_idx = current_code_len_bits / 8;
        int bit_offset = current_code_len_bits % 8;
        unsigned char original_byte_value = current_code_bytes[byte_idx];

        current_code_bytes[byte_idx] |= (1 << (7 - bit_offset));
        generate_huffman_codes(ctx, node->right, current_code_bytes, current_code_len_bits + 1);
        current_code_bytes[byte_idx] = original_byte_value;
    }
}

void build_huffman_tree_and_codes(HuffmanContext* ctx, const char* input_text, size_t input_len) {
    int freq[MAX_CHARS] = {0};

    // Подсчет частот символов
    for (size_t i = 0; i < input_len; ++i) {
        freq[(unsigned char)input_text[i]]++;
    }

    // Создание узлов для каждого символа
    ctx->min_heap_size = 0;
    for (int i = 0; i < MAX_CHARS; ++i) {
        if (freq[i] > 0) {
            pointer_huffman node = create_huffman_node((char)i, freq[i]);
            if (ctx->min_heap_size == ctx->min_heap_capacity) {
                ctx->min_heap_capacity *= 2;
                pointer_huffman* new_nodes = (pointer_huffman*)realloc(ctx->min_heap_nodes, 
                    ctx->min_heap_capacity * sizeof(pointer_huffman));
                if (new_nodes == NULL) exit(EXIT_FAILURE);
                ctx->min_heap_nodes = new_nodes;
            }
            ctx->min_heap_nodes[ctx->min_heap_size++] = node;
        }
    }

    // Особый случай: только один символ
    if (ctx->min_heap_size == 1) {
        pointer_huffman single_node = ctx->min_heap_nodes[0];
        ctx->root = create_huffman_node('$', single_node->freq);
        ctx->root->left = single_node;
        
        // Устанавливаем код для этого символа как 0
        HuffmanCodeEntry* entry = &ctx->huffman_codes[(unsigned char)single_node->data];
        entry->code_len_bits = 1;
        entry->code_bytes[0] = 0;
        return;
    }

    // Построение дерева Хаффмана
    while (ctx->min_heap_size > 1) {
        qsort(ctx->min_heap_nodes, ctx->min_heap_size, sizeof(pointer_huffman), compare_nodes);

        pointer_huffman left = ctx->min_heap_nodes[0];
        pointer_huffman right = ctx->min_heap_nodes[1];

        pointer_huffman top = create_huffman_node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        ctx->min_heap_nodes[0] = top;
        for (int i = 2; i < ctx->min_heap_size; ++i) {
            ctx->min_heap_nodes[i - 1] = ctx->min_heap_nodes[i];
        }
        ctx->min_heap_size--;
    }

    ctx->root = ctx->min_heap_nodes[0];
    
    // Генерация кодов
    unsigned char code_buffer[MAX_CODE_LEN / 8 + 1] = {0};
    generate_huffman_codes(ctx, ctx->root, code_buffer, 0);
}

void bit_buffer_init(BitBuffer* bb, size_t initial_capacity) {
    bb->buffer = (unsigned char*)malloc(initial_capacity);
    if (bb->buffer == NULL) {
        exit(EXIT_FAILURE);
    }
    bb->capacity = initial_capacity;
    bb->data_len = 0;
    bb->current_byte_idx = 0;
    bb->current_bit_offset = 0;
}

void bit_buffer_destroy(BitBuffer* bb) {
    if (bb->buffer != NULL) {
        free(bb->buffer);
        bb->buffer = NULL;
    }
    bb->capacity = 0;
    bb->data_len = 0;
    bb->current_byte_idx = 0;
    bb->current_bit_offset = 0;
}

bool bit_buffer_write_bit(BitBuffer* bb, int bit) {
    if (bb->current_byte_idx >= bb->capacity) {
        size_t new_capacity = bb->capacity * 2;
        unsigned char* new_buffer = (unsigned char*)realloc(bb->buffer, new_capacity);
        if (new_buffer == NULL) {
            return false;
        }
        bb->buffer = new_buffer;
        bb->capacity = new_capacity;
    }
    
    if (bb->current_bit_offset == 0) {
        bb->buffer[bb->current_byte_idx] = 0;
    }

    if (bit) {
        bb->buffer[bb->current_byte_idx] |= (1 << (7 - bb->current_bit_offset));
    }

    bb->current_bit_offset++;
    if (bb->current_bit_offset == 8) {
        bb->current_bit_offset = 0;
        bb->current_byte_idx++;
    }
    
    bb->data_len = bb->current_byte_idx + (bb->current_bit_offset > 0 ? 1 : 0);
    return true;
}

bool bit_buffer_write_byte(BitBuffer* bb, unsigned char byte) {
    if (bb->current_bit_offset != 0) {
        return false;
    }
    
    if (bb->current_byte_idx >= bb->capacity) {
        size_t new_capacity = bb->capacity * 2;
        unsigned char* new_buffer = (unsigned char*)realloc(bb->buffer, new_capacity);
        if (new_buffer == NULL) {
            return false;
        }
        bb->buffer = new_buffer;
        bb->capacity = new_capacity;
    }

    bb->buffer[bb->current_byte_idx++] = byte;
    bb->data_len = bb->current_byte_idx;
    return true;
}

void bit_buffer_finalize_write(BitBuffer* bb) {
    bb->data_len = bb->current_byte_idx + (bb->current_bit_offset != 0 ? 1 : 0);
}

bool bit_buffer_read_bit(BitBuffer* bb, int* bit) {
    if (bb->current_byte_idx >= bb->data_len) {
        return false;
    }

    *bit = (bb->buffer[bb->current_byte_idx] >> (7 - bb->current_bit_offset)) & 1;
    bb->current_bit_offset++;
    if (bb->current_bit_offset == 8) {
        bb->current_bit_offset = 0;
        bb->current_byte_idx++;
    }
    return true;
}

bool bit_buffer_read_byte(BitBuffer* bb, unsigned char* byte) {
    if (bb->current_bit_offset != 0) {
        return false;
    }

    if (bb->current_byte_idx >= bb->data_len) {
        return false;
    }

    *byte = bb->buffer[bb->current_byte_idx++];
    return true;
}

unsigned char* huffman_encode_to_memory(HuffmanContext* ctx, const char* input_text, size_t* compressed_size, size_t input_len) {
    build_huffman_tree_and_codes(ctx, input_text, input_len);

    BitBuffer write_bb;
    bit_buffer_init(&write_bb, BUFFER_SIZE);

    // Записываем длину исходных данных (4 байта)
    if (!bit_buffer_write_byte(&write_bb, (input_len >> 24) & 0xFF) ||
        !bit_buffer_write_byte(&write_bb, (input_len >> 16) & 0xFF) ||
        !bit_buffer_write_byte(&write_bb, (input_len >> 8) & 0xFF) ||
        !bit_buffer_write_byte(&write_bb, input_len & 0xFF)) {
        bit_buffer_destroy(&write_bb);
        return NULL;
    }

    // Записываем количество уникальных символов (2 байта)
    int unique_chars = 0;
    for (int i = 0; i < MAX_CHARS; ++i) {
        if (ctx->huffman_codes[i].code_len_bits > 0) {
            unique_chars++;
        }
    }
    if (!bit_buffer_write_byte(&write_bb, (unique_chars >> 8) & 0xFF) ||
        !bit_buffer_write_byte(&write_bb, unique_chars & 0xFF)) {
        bit_buffer_destroy(&write_bb);
        return NULL;
    }

    // Записываем таблицу кодов
    for (int i = 0; i < MAX_CHARS; ++i) {
        if (ctx->huffman_codes[i].code_len_bits > 0) {
            if (!bit_buffer_write_byte(&write_bb, (unsigned char)i) ||
                !bit_buffer_write_byte(&write_bb, ctx->huffman_codes[i].code_len_bits)) {
                bit_buffer_destroy(&write_bb);
                return NULL;
            }

            size_t code_bytes_len = (ctx->huffman_codes[i].code_len_bits + 7) / 8;
            for (size_t j = 0; j < code_bytes_len; ++j) {
                if (!bit_buffer_write_byte(&write_bb, ctx->huffman_codes[i].code_bytes[j])) {
                    bit_buffer_destroy(&write_bb);
                    return NULL;
                }
            }
        }
    }

    // Кодируем данные
    for (size_t i = 0; i < input_len; ++i) {
        unsigned char c = (unsigned char)input_text[i];
        HuffmanCodeEntry* entry = &ctx->huffman_codes[c];

        for (int j = 0; j < entry->code_len_bits; ++j) {
            int byte_idx = j / 8;
            int bit_offset = j % 8;
            int bit = (entry->code_bytes[byte_idx] >> (7 - bit_offset)) & 1;
            if (!bit_buffer_write_bit(&write_bb, bit)) {
                bit_buffer_destroy(&write_bb);
                return NULL;
            }
        }
    }

    bit_buffer_finalize_write(&write_bb);

    unsigned char* compressed_data = (unsigned char*)malloc(write_bb.data_len);
    if (compressed_data == NULL) {
        bit_buffer_destroy(&write_bb);
        return NULL;
    }
    memcpy(compressed_data, write_bb.buffer, write_bb.data_len);
    *compressed_size = write_bb.data_len;

    bit_buffer_destroy(&write_bb);
    return compressed_data;
}

char* huffman_decode_from_memory(HuffmanContext* ctx, const unsigned char* compressed_data, size_t compressed_size, size_t* decoded_len) {
    BitBuffer read_bb;
    bit_buffer_init(&read_bb, compressed_size);
    memcpy(read_bb.buffer, compressed_data, compressed_size);
    read_bb.data_len = compressed_size;
    read_bb.current_byte_idx = 0;
    read_bb.current_bit_offset = 0;

    // Чтение длины исходных данных
    unsigned char byte_val;
    long long int original_len = 0;
    if (!bit_buffer_read_byte(&read_bb, &byte_val)) goto error;
    original_len = (long long int)byte_val << 24;
    if (!bit_buffer_read_byte(&read_bb, &byte_val)) goto error;
    original_len |= (long long int)byte_val << 16;
    if (!bit_buffer_read_byte(&read_bb, &byte_val)) goto error;
    original_len |= (long long int)byte_val << 8;
    if (!bit_buffer_read_byte(&read_bb, &byte_val)) goto error;
    original_len |= (long long int)byte_val;
    *decoded_len = original_len;

    // Чтение количества уникальных символов
    long long int unique_chars = 0;
    if (!bit_buffer_read_byte(&read_bb, &byte_val)) goto error;
    unique_chars = (long long int)byte_val << 8;
    if (!bit_buffer_read_byte(&read_bb, &byte_val)) goto error;
    unique_chars |= (long long int)byte_val;

    // Очистка предыдущего дерева
    if (ctx->root != NULL) {
        free_huffman_tree(ctx->root);
        ctx->root = NULL;
    }
    ctx->root = create_huffman_node('$', 0);

    // Особый случай: только один символ
    if (unique_chars == 1) {
        unsigned char symbol;
        unsigned char code_len;
        if (!bit_buffer_read_byte(&read_bb, &symbol) ||
            !bit_buffer_read_byte(&read_bb, &code_len)) {
            goto error;
        }

        // Пропускаем байты кода (они не нужны для одного символа)
        size_t code_bytes_len = (code_len + 7) / 8;
        for (size_t j = 0; j < code_bytes_len; ++j) {
            if (!bit_buffer_read_byte(&read_bb, &byte_val)) goto error;
        }

        // Создаем простое дерево с одним символом
        ctx->root->left = create_huffman_node(symbol, 0);
    } 
    else {
        // Построение дерева для нескольких символов
        for (long long int i = 0; i < unique_chars; ++i) {
            unsigned char symbol;
            unsigned char code_len;
            if (!bit_buffer_read_byte(&read_bb, &symbol) ||
                !bit_buffer_read_byte(&read_bb, &code_len)) {
                goto error;
            }

            size_t code_bytes_len = (code_len + 7) / 8;
            unsigned char code_bytes[MAX_CODE_LEN/8 + 1] = {0};
            for (size_t j = 0; j < code_bytes_len; ++j) {
                if (!bit_buffer_read_byte(&read_bb, &code_bytes[j])) {
                    goto error;
                }
            }

            pointer_huffman current = ctx->root;
            for (int j = 0; j < code_len; ++j) {
                int byte_idx = j / 8;
                int bit_offset = j % 8;
                int bit = (code_bytes[byte_idx] >> (7 - bit_offset)) & 1;

                if (bit == 0) {
                    if (current->left == NULL) {
                        current->left = create_huffman_node('$', 0);
                    }
                    current = current->left;
                } else {
                    if (current->right == NULL) {
                        current->right = create_huffman_node('$', 0);
                    }
                    current = current->right;
                }
            }
            current->data = symbol;
        }
    }

    // Декодирование данных
    char* decoded_output = (char*)malloc(original_len);
    if (decoded_output == NULL) goto error;

    pointer_huffman current = ctx->root;
    size_t decoded_count = 0;
    int bit;

    while (decoded_count < original_len) {
        if (!bit_buffer_read_bit(&read_bb, &bit)) {
            free(decoded_output);
            goto error;
        }

        current = (bit == 0) ? current->left : current->right;

        if (current == NULL) {
            free(decoded_output);
            goto error;
        }

        if (current->left == NULL && current->right == NULL) {
            decoded_output[decoded_count++] = current->data;
            current = ctx->root;
        }
    }

    bit_buffer_destroy(&read_bb);
    return decoded_output;

error:
    bit_buffer_destroy(&read_bb);
    if (ctx->root != NULL) {
        free_huffman_tree(ctx->root);
        ctx->root = NULL;
    }
    return NULL;
}