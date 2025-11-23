#include "validation.h"

int valid_expression(char* expression) {
  int result = 1;
  if (!valid_brackets(expression)) {
    result = 0;
    printf("not valid ratio of brackets\n");
  } else if (!valid_orders(expression)) {
    result = 0;
    printf("not valid orders\n");
  } else if (!valid_trigonometry(expression)) {
    result = 0;
    printf("not valid trigonometry\n");
  }
  return result;
}
int valid_brackets(char* expression) {
  int result = 1;
  Stack stack = stack_init();
  for (size_t i = 0; i < strlen(expression); i++) {
    if (expression[i] == '(') {
      push(&stack, '(');
    }
    if (expression[i] == ')') {
      if (pop(&stack) != '(') {
        result = 0;
        break;
      }
    }
  }
  if (!is_empty(&stack)) {
    result = 0;
  }
  destroy_stack(&stack);
  return result;
}
int valid_orders(char* expression) {
  int result = 1;
  char* operators = "+-/*";
  char* not_valid_started_operators = "/*";
  char* valid_started_operators = "+-";
  int expected_operand = 0;
  if (strchr(not_valid_started_operators, expression[0])) {
    result = 0;
  } else {
    for (size_t i = 0; i < strlen(expression); i++) {
      if (expression[i] == ' ') {
        continue;
      }
      if (expression[i] == '(') {
        expected_operand = 1;
      } else if (expression[i] == ')') {
        expected_operand = 0;
      } else if (strchr(operators, expression[i])) {
        if (expected_operand &&
            !strchr(valid_started_operators, expression[i])) {
          result = 0;
        }
        expected_operand = 1;
      } else if (is_digit(expression[i])) {
        expected_operand = 0;
        while (is_digit(expression[i])) {
          i++;
        }
      } else if (expression[i] == 'x') {
        expected_operand = 0;
      }
    }
  }
  if (expected_operand) {
    result = 0;
  }
  return result;
}
int valid_trigonometry(char* expression) {
  int result = 1;
  char* valid_operators = "+-*/()";
  char* valid_started = "cstl";
  char* array_trigonometry[] = {"sin", "cos", "tg", "ctg", "sqrt", "ln"};
  for (size_t i = 0; i < strlen(expression) && result; i++) {
    if (expression[i] >= 'a' && expression[i] <= 'z') {
      if (strchr(valid_started, expression[i])) {
        if (check_correct(expression + i, array_trigonometry)) {
          while (expression[i] >= 'a' && expression[i] <= 'z') {
            i++;
          }
        } else {
          result = 0;
        }
      } else if (!strchr(valid_operators, expression[i]) &&
                 !is_digit(expression[i]) && !(expression[i] == 'x')) {
        printf("not valid character = %c\n", expression[i]);
        result = 0;
      }
    }
  }
  return result;
}
int check_correct(char* expression, char* array[]) {
  char* checked_expression = malloc(5 * sizeof(char));
  for (int i = 0; i < 5; i++) {
    if (expression[i] == '\0') {
      checked_expression[i] = '\0';
      break;
    }
    checked_expression[i] = expression[i];
    if (i == 4) {
      checked_expression[i] = '\0';
    }
  }
  int result = 0;
  for (int i = 0; i < 6; i++) {
    if (strstr(checked_expression, array[i]) != NULL) {
      result = 1;
      break;
    }
  }
  free(checked_expression);
  return result;
}