#pragma once
#include <stdio.h>
#include <stdlib.h>
#define HEIGHT 25
#define WIDTH 80

void draw_graph(int** matrix);
int** init_matrix(char* expression);
int** alloc_matrix();
void free_matrix(int** matrix);