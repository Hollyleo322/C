#pragma once
#include "polish_notation.h"
#include "stack.h"
#include "stdlibs.h"

int valid_expression(char* expression);
int valid_brackets(char* expression);
int valid_orders(char* expression);
int valid_trigonometry(char* expression);
int check_correct(char* expression, char* array[]);