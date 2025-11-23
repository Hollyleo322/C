#pragma once
#include "stdlibs.h"
#define STARTED_STACK_SIZE 10

typedef struct Double_Stack {
  double* array;
  int top;
  int size;
} Double_Stack;

Double_Stack double_stack_init();
void push_double(Double_Stack* stack, double value);
double pop_double(Double_Stack* stack);
double top_double(Double_Stack* stack);
void erase_top_double(Double_Stack* stack);
int is_empty_double(Double_Stack* stack);
void print_stack_double(Double_Stack* stack);
void destroy_double_stack(Double_Stack* stack);