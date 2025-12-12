#ifndef stack_h
#define stack_h

typedef struct Stack Stack;

Stack* stack_create(void);
void stack_free(Stack* s);
void stack_push(Stack* s, int val);
int stack_pop(Stack* s, int* out);

#endif