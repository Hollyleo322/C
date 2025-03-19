#include "s21_matrix.h"
int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int res = 0;
  if (rows <= 0 || columns <= 0 || !result) {
    res = 1;
  } else {
    result->matrix = malloc(sizeof(double *) * rows);
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = calloc(columns, sizeof(double));
      if (!result->matrix[i]) {
        res = 1;
        break;
      }
    }
    result->rows = rows;
    result->columns = columns;
    if (!result->matrix) {
      res = 1;
    }
  }
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  if (A && A->matrix && A->rows > 0 && A->columns > 0) {
    for (int i = 0; i < A->rows; i++) {
      if (A->matrix[i]) {
        free(A->matrix[i]);
      }
    }
    free(A->matrix);
    A->columns = 0;
    A->rows = 0;
    A->matrix = NULL;
  }
}
int check_matrix(matrix_t *number) {
  int res = 0;
  if (!number->matrix || number->columns <= 0 || number->rows <= 0) {
    res = 1;
  } else {
    for (int i = 0; i < number->rows; i++) {
      if (res) {
        break;
      }
      for (int j = 0; j < number->columns; j++) {
        if (number->matrix[i] == NULL || number->matrix[i][j] == -INFINITY ||
            number->matrix[i][j] == INFINITY || isnan(number->matrix[i][j])) {
          res = 1;
        }
      }
    }
  }
  return res;
}
int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = SUCCESS;
  if (!A || !B || A->rows != B->rows || A->columns != B->columns ||
      check_matrix(A) || check_matrix(B)) {
    res = FAILURE;
  } else {
    for (int i = 0; i < A->rows; i++) {
      if (!res) {
        break;
      }
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
          res = FAILURE;
        }
      }
    }
  }
  return res;
}
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;
  if (!A || !B || check_matrix(A) || check_matrix(B) || !result) {
    res = 1;
  } else if (A->columns != B->columns || A->rows != B->rows) {
    res = 2;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return res;
}
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;
  if (!A || !B || check_matrix(A) || check_matrix(B) || !result) {
    res = 1;
  } else if (A->columns != B->columns || A->rows != B->rows) {
    res = 2;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return res;
}
int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = 0;
  if (!A || check_matrix(A) || number == -INFINITY || number == INFINITY ||
      isnan(number) || !result) {
    res = 1;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return res;
}
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;
  if (!A || !B || check_matrix(A) || check_matrix(B) || !result) {
    res = 1;
  } else if (A->columns != B->rows) {
    res = 2;
  } else {
    s21_create_matrix(A->rows, B->columns, result);
    int value = A->columns;
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        result->matrix[i][j] = multymatrix(A->matrix, B->matrix, i, j, value);
      }
    }
  }
  return res;
}
double multymatrix(double **matrix_1, double **matrix_2, int row, int column,
                   int value_for_cycle) {
  double res = 0, c = 0;
  for (int i = 0; i < value_for_cycle; i++) {
    c = matrix_1[row][i] * matrix_2[i][column];
    res += c;
  }
  return res;
}
int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = 0;
  if (!A || check_matrix(A) || !result) {
    res = 1;
  } else {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->columns; i++) {
      for (int j = 0; j < A->rows; j++) {
        result->matrix[i][j] = A->matrix[j][i];
      }
    }
  }
  return res;
}
matrix_t minor_matrix(matrix_t *A, int row, int column) {
  matrix_t res;
  s21_create_matrix(A->rows - 1, A->columns - 1, &res);
  int k = 0, s = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i == row) {
      continue;
    }
    s = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j != column) {
        res.matrix[k][s] = A->matrix[i][j];
        s += 1;
      }
    }
    k += 1;
  }
  return res;
}
double get_determinant(matrix_t *A) {
  double res = 0;
  matrix_t minor;
  int sign = -1;
  if (A->columns == 2 && A->rows == 2) {
    res = A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else if (A->columns == 1 && A->rows == 1) {
    res = A->matrix[0][0];
  } else {
    for (int i = 0; i < A->columns; i++) {
      minor = minor_matrix(A, 0, i);
      res += pow(sign, i) * A->matrix[0][i] * get_determinant(&minor);
      s21_remove_matrix(&minor);
    }
  }
  return res;
}
int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int res = 0;
  if (!A || check_matrix(A) || !result) {
    res = 1;
  } else if (A->columns != A->rows) {
    res = 2;
  } else if (A->columns > 1 && A->rows > 1) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        matrix_t minor = minor_matrix(A, i, j);
        result->matrix[i][j] = get_determinant(&minor);
        result->matrix[i][j] *= pow(-1, i + j);
        s21_remove_matrix(&minor);
      }
    }
  } else {
    s21_create_matrix(1, 1, result);
    result->matrix[0][0] = A->matrix[0][0];
  }
  return res;
}
int s21_determinant(matrix_t *A, double *result) {
  int res = 0;
  if (!A || check_matrix(A) || !result) {
    res = 1;
  } else if (A->columns != A->rows) {
    res = 2;
  } else {
    *result = get_determinant(A);
  }
  return res;
}
int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int res = 0;
  if (!A || !result || check_matrix(A)) {
    res = 1;
  } else if (A->columns != A->rows || !get_determinant(A)) {
    res = 2;
  } else if (A->columns == 1 && A->rows == 1) {
    s21_create_matrix(1, 1, result);
    result->matrix[0][0] = 1 / get_determinant(A);
  } else {
    matrix_t comp, transpose;
    double deter = get_determinant(A);
    s21_calc_complements(A, &comp);
    s21_transpose(&comp, &transpose);
    s21_mult_number(&transpose, 1 / deter, result);
    s21_remove_matrix(&comp);
    s21_remove_matrix(&transpose);
  }
  return res;
}