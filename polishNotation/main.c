#include <stdio.h>

#include "polish_notation.h"

int main(void) {
  char* expression = NULL;
  size_t n = 0;
  getline(&expression, &n, stdin);
  char* polish = get_polish_notation_from_expression(expression);
  printf("%s", polish);
  free(expression);
  free(polish);
  return 0;
}