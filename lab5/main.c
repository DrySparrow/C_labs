#include "huffman.h"
#include <limits.h>

char* read_file_as_string(const char* filename, size_t * len) {
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    size_t capacity = 128;
    char* buffer = malloc(capacity);
    if (!buffer) { fclose(file); return NULL; }

    *len = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (*len + 1 >= capacity) {
            capacity *= 2;
            char* new_buffer = realloc(buffer, capacity);
            if (!new_buffer) { free(buffer); fclose(file); return NULL; }
            buffer = new_buffer;
        }
        buffer[(*len)++] = (char)c;
    }
    //buffer[*len] = '\0'; // Не влияет на данные, так как len учитывает только прочитанные байты
    fclose(file);
    return buffer;
}

unsigned char* read_file_as_binary(const char* filename, size_t* file_size) {
    FILE* file = fopen(filename, "rb");
    if (!file) { *file_size = 0; return NULL; }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* buffer = malloc(size);
    if (!buffer) { fclose(file); *file_size = 0; return NULL; }

    *file_size = fread(buffer, 1, size, file);
    fclose(file);
    return buffer;
}

void write_binary_to_file(const char* filename, const unsigned char* data, size_t size) {
    FILE* file = fopen(filename, "wb");
    if (file) {
        fwrite(data, 1, size, file);
        fclose(file);
    }
}

// Измененная функция для записи с учетом длины
void write_text_to_file(const char* filename, const char* text, size_t len) {
    FILE* file = fopen(filename, "wb");
    if (file && text) {
        fwrite(text, 1, len, file);
        fclose(file);
    }
}

int main() {
    HuffmanContext h_ctx;
    huffman_context_init(&h_ctx);

    size_t len_full_input = 0;
    char* full_input = read_file_as_string("in.txt", &len_full_input);
    if (!full_input || len_full_input < 2) {
        free(full_input);
        huffman_context_destroy(&h_ctx);
        return EXIT_SUCCESS;
    }

    char choice = full_input[0];
    char* data = full_input + 1;
    size_t data_len = len_full_input - 1;

    if (choice == 'c') {
        size_t compressed_size;
        unsigned char* compressed = huffman_encode_to_memory(&h_ctx, data, &compressed_size, data_len);
        if (compressed) {
            write_binary_to_file("out.txt", compressed, compressed_size);
            free(compressed);
        }
    } else if (choice == 'd') {
        FILE* file = fopen("in.txt", "rb");
        if (!file) { free(full_input); huffman_context_destroy(&h_ctx); return EXIT_SUCCESS; }

        fseek(file, 1, SEEK_SET);
        size_t compressed_size;
        fseek(file, 0, SEEK_END);
        compressed_size = ftell(file) - 1;
        fseek(file, 1, SEEK_SET);

        unsigned char* compressed = malloc(compressed_size);
        if (compressed) {
            fread(compressed, 1, compressed_size, file);
            size_t decoded_len;
            char* decoded = huffman_decode_from_memory(&h_ctx, compressed, compressed_size, &decoded_len);
            if (decoded) {
                write_text_to_file("out.txt", decoded, decoded_len);
                free(decoded);
            }
            free(compressed);
        }
        fclose(file);
    }

    free(full_input);
    huffman_context_destroy(&h_ctx);
    return EXIT_SUCCESS;
}