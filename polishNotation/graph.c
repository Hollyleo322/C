#include "graph.h"

void draw_graph(int** matrix) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (matrix[i][j] == 1) {
        printf("*");
      } else {
        printf(".");
      }
    }
  }
}
int** alloc_matrix() {
  int** matrix = malloc(HEIGHT * sizeof(int*));
  for (int i = 0; i < HEIGHT; i++) {
    matrix[i] = malloc(WIDTH * sizeof(int));
  }
  return matrix;
}
void free_matrix(int** matrix) {
  for (int i = 0; i < HEIGHT; i++) free(matrix[i]);
  free(matrix);
}
int** init_matrix(char* expression) { return }