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
    printf("\n");
  }
}
int** alloc_matrix() {
  int** matrix = calloc(HEIGHT, sizeof(int*));
  for (int i = 0; i < HEIGHT; i++) {
    matrix[i] = calloc(WIDTH, sizeof(int));
  }
  return matrix;
}
void free_matrix(int** matrix) {
  for (int i = 0; i < HEIGHT; i++) free(matrix[i]);
  free(matrix);
}
int** init_matrix(char* polish_notation, int** matrix) {
  double step_x = 12.57 / (WIDTH - 1), x = 0.0;
  double* array_y = malloc(HEIGHT * sizeof(double));
  init_array_y(array_y);
  for (int i = 0; i < WIDTH; i++) {
    double y = get_y_by_polish_notation(polish_notation, x);
    if (y <= 1.0 && y >= -1.0) {
      int index = get_index_by_y(y, array_y);
      if (index != -1) {
        matrix[index][i] = 1;
      }
    }
    x += step_x;
  }
  free(array_y);
  return matrix;
}
void init_array_y(double* array) {
  double step_y = 2.0 / 24, y = -1.0;
  for (int i = 0; i < HEIGHT; i++) {
    array[i] = y;
    y += step_y;
  }
}
int get_index_by_y(double y, double* array) {
  int index = -1;
  for (int i = 0; i < HEIGHT; i++) {
    if (rounded_y(y) == rounded_y(array[i])) {
      index = i;
      break;
    }
  }
  return index;
}
double rounded_y(double number) { return round(number * 10) / 10; }
