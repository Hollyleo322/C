#pragma once
#include "double_stack.h"
#include "stack.h"
#include "stdlibs.h"
#define MAX 200
// ! - unary minus
// sin - s
// cos - c
// tg - t
// ctg - k
// sqrt - r
// ln - l

int processing_operators(Stack* stack, char operator, char next_operator,
                         char* polish_notation, int* counter, int index,
                         char* prev);
int get_priority_by_operator(char operator);
char is_digit(char c);
char* get_polish_notation_from_expression(char* expression);
int processing_trigonometry(Stack* stack, char operator, char next_operator,
                            int index);
int processing_s(Stack* stack, int index, char operator, char next_operator);
int processing_c(Stack* stack, int index, char operator, char next_operator);
int processing_t(Stack* stack, int index);
int processing_l(Stack* stack, int index);
double get_y_by_polish_notation(char* polish_notation, double x);
int len_int(int number);
void operation(Double_Stack* stack, int operator, double x);