#pragma once
#include <stdio.h>
#include <stdlib.h>
#define STARTED_STACK_SIZE 10

typedef struct Stack {
  char *array;
  int top;
  int size;
} Stack;

Stack stack_init();
void push(Stack *stack, char value);
char pop(Stack *stack);
void destroy_stack(Stack *stack);
int is_empty(Stack *stack);
char top(Stack *stack);
void print_stack(Stack *stack);