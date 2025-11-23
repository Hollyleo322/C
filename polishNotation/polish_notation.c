#include "polish_notation.h"

char* get_polish_notation_from_expression(char* expression) {
  Stack stack = stack_init();
  char* polish_notation = malloc(sizeof(char) * MAX);
  int counter = 0;
  char prev = -1;
  for (size_t i = 0; i < strlen(expression); i++) {
    if (expression[i] == ' ') {
      continue;
    }
    if (is_digit(expression[i]) || expression[i] == 'x') {
      polish_notation[counter++] = expression[i];
      prev = expression[i];
    } else {
      i = processing_operators(&stack, expression[i], expression[i + 1],
                               polish_notation, &counter, i, &prev);
    }
  }
  if (!is_empty(&stack)) {
    while (!is_empty(&stack)) {
      polish_notation[counter++] = pop(&stack);
    }
  }
  // printf("polish notation = %s\n", polish_notation);
  polish_notation[counter] = '\0';
  destroy_stack(&stack);
  return polish_notation;
}
char is_digit(char c) {
  char result = 0;
  if (c >= '0' && c <= '9') result = 1;
  return result;
}
int processing_operators(Stack* stack, char operator, char next_operator,
                         char* polish_notation, int* counter, int index,
                         char* prev) {
  char* trigonometry = "sctl";
  if (operator == '-' && *prev == -1) {
    push(stack, '!');
  } else if (operator == '*' || operator == '+' || operator == '/' ||
             operator == '-') {
    polish_notation[(*counter)++] = ' ';
    while (!is_empty(stack) &&
           get_priority_by_operator(operator) <=
               get_priority_by_operator(top(stack)) &&
           !(top(stack) == '(')) {
      polish_notation[(*counter)++] = pop(stack);
    }
    push(stack, operator);
    if (*prev != -1) {
      *prev = -1;
    }
  } else if (operator == '(') {
    push(stack, operator);
  } else if (operator == ')') {
    while (!(top(stack) == '(')) {
      polish_notation[(*counter)++] = pop(stack);
    }
    pop(stack);
  } else if (strchr(trigonometry, operator)) {
    index = processing_trigonometry(stack, operator, next_operator, index);
  }
  return index;
}
int get_priority_by_operator(char operator) {
  int result = 1;
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
  } else if (next_operator == 'q') {
    push(stack, 'r');
    index += 3;
  }
  return index;
}

int processing_c(Stack* stack, int index, char operator, char next_operator) {
  if (next_operator == 'o') {
    push(stack, operator);
  } else if (next_operator == 't') {
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
double get_y_by_polish_notation(char* polish_notation, double x) {
  Double_Stack stack = double_stack_init();
  char* operators = "sctkrl+-/*!";
  for (size_t i = 0; i < strlen(polish_notation); i++) {
    if (is_digit(polish_notation[i])) {
      double number = atof(polish_notation + i);
      push_double(&stack, number);
      i += len_int((int)number);
    } else if (polish_notation[i] == 'x') {
      push_double(&stack, -INFINITY);
    } else if (strchr(operators, polish_notation[i])) {
      operation(&stack, polish_notation[i], x);
    }
  }
  double result = pop_double(&stack);
  destroy_double_stack(&stack);
  return result;
}
int len_int(int number) {
  int counter = 0;
  while (number > 10) {
    number /= 10;
    counter++;
  }
  return counter;
}
void operation(Double_Stack* stack, int operator, double x) {
  double first = pop_double(stack), second = 0.;
  if (first == -INFINITY) {
    first = x;
  }
  switch (operator) {
    case '!':
      push_double(stack, first * -1);
      break;
    case '+':
      second = pop_double(stack);
      push_double(stack, first + second);
      break;
    case '-':
      second = pop_double(stack);
      push_double(stack, second - first);
      break;
    case '*':
      second = pop_double(stack);
      push_double(stack, first * second);
      break;
    case '/':
      second = pop_double(stack);
      if (first != 0) {
        push_double(stack, second / first);
      }
      break;
    case 's':
      push_double(stack, sin(first));
      break;
    case 'c':
      push_double(stack, cos(first));
      break;
    case 't':
      push_double(stack, tan(first));
      break;
    case 'k':
      push_double(stack, 1.0 / tan(first));
      break;
    case 'r':
      push_double(stack, sqrt(first));
      break;
    case 'l':
      push_double(stack, log(first));
      break;
  }
}