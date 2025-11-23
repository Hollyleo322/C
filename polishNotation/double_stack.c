#include "double_stack.h"

Double_Stack double_stack_init() {
  Double_Stack stack;
  stack.array = calloc(STARTED_STACK_SIZE, sizeof(double));
  stack.top = 0;
  stack.size = STARTED_STACK_SIZE;
  return stack;
}

void push_double(Double_Stack* stack, double value) {
  if (stack->top == stack->size - 1) {
    stack->array = realloc(stack->array, stack->size * 2);
    stack->size *= 2;
  }
  stack->top++;
  stack->array[stack->top] = value;
}
double pop_double(Double_Stack* stack) {
  double result = 0;
  if (stack->top > 0) {
    result = stack->array[stack->top];
    stack->array[stack->top] = 0;
    stack->top--;
  }
  return result;
}
double top_double(Double_Stack* stack) {
  if (stack->top > 0) {
    return stack->array[stack->top];
  }
  return 0;
}
void erase_top_double(Double_Stack* stack) { stack->array[stack->top--] = 0; }
void destroy_double_stack(Double_Stack* stack) { free(stack->array); }

int is_empty_double(Double_Stack* stack) { return stack->top == 0; }
void print_stack_double(Double_Stack* stack) {
  printf("print double stack\n");
  for (int i = 1; i <= stack->top; i++) {
    printf("double = %lf index = %i\n", stack->array[i], i);
  }
  printf("\n");
}