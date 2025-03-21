#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define SUCCESS 1
#define FAILURE 0
#define INCORRECT_MATIX_ERROR 1

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int check_matrix(matrix_t *number);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
double multymatrix(double **matrix_1, double **matrix_2, int row, int column,
                   int value_for_cycle);
int s21_transpose(matrix_t *A, matrix_t *result);
matrix_t minor_matrix(matrix_t *A, int row, int column);
double get_determinant(matrix_t *A);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);