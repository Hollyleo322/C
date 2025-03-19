#include "s21_tetris.h"

void init_field(GameInfo_t *var, int number) {
  for (int i = 1; i < HEIGHT_MAIN; i++) {
    for (int j = 0; j < WIDTH_MAIN; j++) {
      var->field[i][j] = number;
    }
  }
}