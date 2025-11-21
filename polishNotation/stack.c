#include "stack.h"

Stack stack_init() {
  Stack stack;
  stack.array = calloc(STARTED_STACK_SIZE, sizeof(char));
  stack.top = 0;
  stack.size = STARTED_STACK_SIZE;
  return stack;
}

void push(Stack *stack, char value) {
  printf("push\n");
  if (stack->top == stack->size - 1) {
    stack->array = realloc(stack->array, stack->size * 2);
    stack->size *= 2;
  }
  stack->top++;
  stack->array[stack->top] = value;
  print_stack(stack);
}
char pop(Stack *stack) {
  printf("pop\n");
  char result = 0;
  if (stack->top > 0) {
    result = stack->array[stack->top];
    stack->array[stack->top] = 0;
    stack->top--;
  }
  print_stack(stack);
  return result;
}
char top(Stack *stack) {
  if (stack->top > 0) {
    return stack->array[stack->top];
  }
  return 0;
}
void erase_top(Stack *stack) { stack->array[stack->top--] = 0; }
void destroy_stack(Stack *stack) { free(stack->array); }

int is_empty(Stack *stack) { return stack->top == 0; }
void print_stack(Stack *stack) {
  printf("print stack\n");
  for (int i = 1; i <= stack->top; i++) {
    printf("char = %c int = %d index = %i\n", stack->array[i], stack->array[i],
           i);
  }
  printf("\n");
}