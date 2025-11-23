#include "graph.h"
#include "polish_notation.h"
#include "validation.h"

int main(void) {
  char* expression = NULL;
  size_t n = 0;
  getline(&expression, &n, stdin);
  if (valid_expression(expression)) {
    char* polish = get_polish_notation_from_expression(expression);
    int** matrix = alloc_matrix();
    matrix = init_matrix(polish, matrix);
    draw_graph(matrix);
    free_matrix(matrix);
    free(polish);
  } else {
    printf("Not valid given expression\n");
  }
  free(expression);
  return 0;
}