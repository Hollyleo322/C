#pragma once

#include <stdio.h>
#include <string.h>

#include "stack.h"

// sin - s
// cos - c
// tg - t
// ctg - k
// sqrt - r
// ln - l

int processing_operators(Stack* stack, char operator, char next_operator,
                         char* polish_notation, int counter, int index,
                         char* prev);
int get_priority_by_operator(char operator);
int is_digit(char c);
char* get_polish_notation_from_expression(char* expression);
int processing_trigonometry(Stack* stack, char operator, char next_operator,
                            int index);
int processing_s(Stack* stack, int index, char operator, char next_operator);
int processing_c(Stack* stack, int index, char operator, char next_operator);
int processing_t(Stack* stack, int index);
int processing_l(Stack* stack, int index);