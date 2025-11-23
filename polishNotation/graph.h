#pragma once
#include "polish_notation.h"
#include "stdlibs.h"
#define HEIGHT 25
#define WIDTH 80

void draw_graph(int** matrix);
int** init_matrix(char* polish_notation, int** matrix);
int** alloc_matrix();
void free_matrix(int** matrix);
void init_array_y(double* array);
int get_index_by_y(double y, double* array);
double rounded_y(double number);