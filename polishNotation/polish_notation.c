#include "polish_notation.h"

char* get_polish_notation_from_expression(char* expression) {
  Stack stack = stack_init();
  char* polish_notation = malloc(sizeof(char) * (strlen(expression) + 1));
  int counter = 0;
  char prev = -1;
  for (size_t i = 0; i < strlen(expression); i++) {
    if (is_digit(expression[i])) {
      polish_notation[counter++] = expression[i];
      prev = expression[i];
    } else {
      i = processing_operators(&stack, expression[i], expression[i + 1],
                               polish_notation, counter, i, &prev);
    }
  }
  if (!is_empty(&stack)) {
    printf("ok\n");
    printf("%d\n", polish_notation[counter++] = pop(&stack));
    printf("%d\n", counter);
  }
  polish_notation[counter] = '\0';
  destroy_stack(&stack);
  return polish_notation;
}
int is_digit(char c) {
  char result = 0;
  if (c >= '0' && c <= '9') result = 1;
  return result;
}
int processing_operators(Stack* stack, char operator, char next_operator,
                         char* polish_notation, int counter, int index,
                         char* prev) {
  if (operator == '-' && *prev == -1) {
    push(stack, '!');
    printf("unary minus = %c\n", top(stack));
  } else if (operator == '*' || operator == '+' || operator == '/' ||
             operator == '-') {
    while (!is_empty(stack) && get_priority_by_operator(operator) >=
                                   get_priority_by_operator(top(stack))) {
      polish_notation[counter++] = pop(stack);
    }
    push(stack, operator);
    // printf("real operator = %c\n", operator);
    // printf("in processing operator = %c\n", top(stack));
    if (operator == '-' && *prev != -1) {
      *prev = -1;
    }
  } else if (operator == '(') {
    push(stack, operator);
    // printf("opened ( operator = %c\n", operator);
    // printf("in processing operator = %c\n", top(stack));
  } else if (operator == ')') {
    printf("closed ) operator = %c\n", operator);
    // printf("in processing operator = %c\n", top(stack));
    while (!(top(stack) == '(')) {
      if (top(stack) == 0) {
        break;
      }
      printf("while %c\n", top(stack));
      polish_notation[counter++] = pop(stack);
    }
    printf("after while %c\n", top(stack));
  } else if (operator >= 'a' && operator <= 'z') {
    index = processing_trigonometry(stack, operator, next_operator, index);
  }
  return index;
}
int get_priority_by_operator(char operator) {
  char result = 1;
  if (operator == '(') {
    result = 4;
  }
  if (operator == '!') {
    result = 3;
  }
  if (operator == '/' || operator == '*') {
    result = 2;
  }
  return result;
}
int processing_trigonometry(Stack* stack, char operator, char next_operator,
                            int index) {
  switch (operator) {
    case 's':
      index = processing_s(stack, index, operator, next_operator);
      break;
    case 'c':
      index = processing_c(stack, index, operator, next_operator);
      break;
    case 't':
      index = processing_t(stack, index);
      break;
    case 'l':
      index = processing_l(stack, index);
      break;
  }
  return index;
}

int processing_s(Stack* stack, int index, char operator, char next_operator) {
  if (next_operator == 'i') {
    index += 2;
    push(stack, operator);
  } else {
    push(stack, 'r');
    index += 3;
  }
  return index;
}

int processing_c(Stack* stack, int index, char operator, char next_operator) {
  if (next_operator == 'o') {
    push(stack, operator);
  } else {
    push(stack, 'k');
  }
  return index += 2;
}
int processing_t(Stack* stack, int index) {
  push(stack, 't');
  return index += 1;
}
int processing_l(Stack* stack, int index) {
  push(stack, 'l');
  return index += 1;
}