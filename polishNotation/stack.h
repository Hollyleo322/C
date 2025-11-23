#pragma once
#include "stdlibs.h"
#define STARTED_STACK_SIZE 10

typedef struct Stack {
  int* array;
  int top;
  int size;
} Stack;

Stack stack_init();
void push(Stack* stack, int value);
int pop(Stack* stack);
void destroy_stack(Stack* stack);
int is_empty(Stack* stack);
int top(Stack* stack);
void print_stack(Stack* stack);