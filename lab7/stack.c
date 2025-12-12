#include <stdlib.h>

#include "stack.h"

#define BLOCK_SIZE 1

typedef struct Block {
    struct Block* prev;
    int* elements;
    size_t count;
} Block;

struct Stack {
    Block* top;
    size_t total;
};


static Block* create_block(Block* previous) {
    Block* b = malloc(sizeof(Block));
    if (b == NULL)
        exit(0);
    b->prev = previous;
    b->elements = malloc(sizeof(int));
    if (b->elements == NULL)
        exit(0);
    b->count = 0;
    
    return b;
}

Stack* stack_create() {
    Stack* s = malloc(sizeof(Stack));
    if (s == NULL)
        exit(0);
    s->top = create_block(NULL);
    s->total = 0;

    return s;
}

void stack_free(Stack* s) {
    Block* current = s->top;
    while(current) {
        Block* temp = current->prev;
        free(current->elements);
        free(current);
        current = temp;
    }
    free(s);
}

void stack_push(Stack* s, int val) {
    if(s->top->count == BLOCK_SIZE) {
        Block* new_block = create_block(s->top);
        s->top = new_block;
    }
    s->top->elements[(s->top->count)++] = val;
    s->total++;
}

int stack_pop(Stack* s, int* out) {
    if(s->top->count == 0) {
        if(!s->top->prev)
            return -1;
        
        Block* old = s->top;
        s->top = s->top->prev;
        free(old->elements);
        free(old);
    }
    *out = s->top->elements[--(s->top->count)];
    s->total--;

    return 0;
}