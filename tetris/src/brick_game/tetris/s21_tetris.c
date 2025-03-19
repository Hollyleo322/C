#include "s21_tetris.h"

void main_game() {
  int end_game = 0, new = 1, count_periods = 0;
  int array_periods[] = {30000, 27500, 25000, 22500, 20000,
                         17500, 15000, 12500, 10000, 7500};
  int period = array_periods[count_periods];
  writening_current_figure(1, 1, 0, 0);
  while (1) {
    end_game = window_of_game(period, new);
    new = 0;
    if (end_game == 1) {
      break;
    }
    if (end_game == 2) {
      count_periods += 1;
    }
    if (!period) {
      period = array_periods[count_periods];
    }
    period -= 1;
  }
}
int init_object(GameInfo_t *var, FILE *file) {
  int res = 0;
  if (!var || !file) {
    res = 1;
  } else {
    res += memory_allocation_field(var);
    res += read_high_score(&var->high_score, file);
    init_ints(var);
    res += memory_allocation_next(var);
    res += init_next_figure(var);
  }
  return res;
}
int memory_allocation_field(GameInfo_t *var) {
  int res = 0;
  if (!var) {
    res = 1;
  } else {
    var->field = malloc(sizeof(int *) * HEIGHT_MAIN);
    for (int i = 0; i < HEIGHT_MAIN; i++) {
      var->field[i] = calloc(WIDTH_MAIN, sizeof(int));
      if (!var->field[i]) {
        res = 1;
        break;
      }
    }
  }
  return res;
}
void free_field(GameInfo_t *var) {
  if (var->field) {
    for (int i = 0; i < HEIGHT_MAIN; i++) {
      free(var->field[i]);
    }
    free(var->field);
  }
}
void free_next(GameInfo_t *var) {
  if (var->next) {
    for (int i = 0; i < VALUE_SAMPLE; i++) {
      free(var->next[i]);
    }
    free(var->next);
  }
}
int read_high_score(int *high_score, FILE *file) {
  int res = 0;
  if (!file || !high_score) {
    res = 1;
  } else {
    fscanf(file, "%d", high_score);
  }
  return res;
}
int init_ints(GameInfo_t *var) {
  int res = 0;
  if (var) {
    var->score = 0;
    var->level = 0;
    var->speed = 0;
    var->pause = 0;
  } else {
    res = 1;
  }
  return res;
}
int init_next_figure(GameInfo_t *var) {
  int res = 0;
  if (var) {
    int samples[][VALUE_SAMPLE][VALUE_SAMPLE] = {
        {{0, 0, 0, 0, 0},
         {0, 1, 1, 1, 1},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},
        {{0, 0, 0, 0, 0},
         {0, 1, 0, 0, 0},
         {0, 1, 1, 1, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},
        {{0, 0, 0, 0, 0},
         {0, 0, 0, 1, 0},
         {0, 1, 1, 1, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},
        {{0, 0, 0, 0, 0},
         {0, 0, 1, 1, 0},
         {0, 0, 1, 1, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},
        {{0, 0, 0, 0, 0},
         {0, 0, 1, 1, 0},
         {0, 1, 1, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},
        {{0, 0, 0, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 1, 1, 1, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},
        {{0, 0, 0, 0, 0},
         {0, 1, 1, 0, 0},
         {0, 0, 1, 1, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},
    };
    srand(time(NULL));
    int random = rand() % COUNT_FIGURES;
    if (!res) {
      for (int i = 0; i < VALUE_SAMPLE; i++) {
        for (int j = 0; j < VALUE_SAMPLE; j++) {
          var->next[i][j] = samples[random][i][j];
        }
      }
    }
  } else {
    res = 1;
  }
  return res;
}
int memory_allocation_next(GameInfo_t *var) {
  int res = 0;
  if (!var) {
    res = 1;
  } else {
    var->next = malloc(sizeof(int *) * VALUE_SAMPLE);
    for (int i = 0; i < VALUE_SAMPLE; i++) {
      var->next[i] = calloc(VALUE_SAMPLE, sizeof(int));
      if (var->next[i] == NULL) {
        res = 1;
        break;
      }
    }
  }
  return res;
}
int init_current_figure(figure *current, int **ptr_next) {
  int res = 0;
  if (!current || !ptr_next) {
    res = 1;
  }
  if (!res) {
    for (int i = 0; i < VALUE_SAMPLE; i++) {
      for (int j = 0; j < VALUE_SAMPLE; j++) {
        current->blocks[i][j] = ptr_next[i][j];
      }
    }
    current->x = WIDTH_MAIN / 2 - 3;
    current->y = 0;
  }
  return res;
}
GameInfo_t updateCurrentState() {
  GameInfo_t res = writening_current_figure(0, 0, 1, 0);
  return res;
}
GameInfo_t writening_current_figure(int new_current, int new_game, int update,
                                    UserAction_t action) {
  static GameInfo_t var;
  if (var.pause == 4) {
    var.pause = 0;
  }
  static int lvl = 0, check_high_score = 0;
  int check_game = 0, check_current = 0, count_pts = 0;
  if (new_game) {
    FILE *file = fopen("brick_game/tetris/high_score.txt", "r+");
    check_game = init_object(&var, file);
    if (!check_game) {
      check_high_score = var.high_score;
      fclose(file);
    }
  }
  static figure current;
  if (new_current && !check_game) {
    memory_allocation_figure(&current);
    check_current = init_current_figure(&current, var.next);
  }
  if (check_current || check_game) {
    var.pause = 2;
  }
  if (update) {
    erase_field(&var, current);
    move_down(&current);
    if (collision(var, current)) {
      move_up(&current);
      put_current(&var, current);
      count_pts = count_points(&var);
      var.score += count_pts;
      lvl += count_pts;
      if (lvl >= 600) {
        var.level += 1;
        lvl = 0;
        var.pause = 4;
      }
      if (var.score > var.high_score) {
        update_score(&var);
      }
      create_new_figure(&var, &current);
      if (collision(var, current)) {
        if (var.score > check_high_score) {
          writening_high_score(var.score);
        }
        free_in_the_end_of_game(&var, &current);
      }
    } else {
      put_current(&var, current);
    }
  } else {
    var = do_action(&var, &current, action, check_high_score);
  }
  return var;
}
int move_down(figure *var) {
  int res = 0;
  if (var) {
    var->y += 1;
  } else {
    res = 1;
  }
  return res;
}
int collision(GameInfo_t var, figure current) {
  int res = 0;
  for (int i = 0; i < VALUE_SAMPLE && !res; i++) {
    for (int j = 0; j < VALUE_SAMPLE; j++) {
      if (current.blocks[i][j] == 1) {
        int x = current.x + j;
        int y = current.y + (i - 1);
        if (x < 0 || x >= WIDTH_MAIN || y < 0 || y >= HEIGHT_MAIN) {
          res = 1;
        } else if (var.field[y][x] == 1) {
          res = 1;
        }
      }
    }
  }
  return res;
}
int move_up(figure *var) {
  int res = 0;
  if (var) {
    var->y -= 1;
  } else {
    res = 1;
  }
  return res;
}
int move_left(figure *var) {
  int res = 0;
  if (var) {
    var->x -= 1;
  } else {
    res = 1;
  }
  return res;
}
int move_right(figure *var) {
  int res = 0;
  if (var) {
    var->x += 1;
  } else {
    res = 1;
  }
  return res;
}
int put_current(GameInfo_t *var, figure current) {
  int res = 0;
  if (var) {
    for (int i = 0; i < VALUE_SAMPLE; i++) {
      for (int j = 0; j < VALUE_SAMPLE; j++) {
        if (current.blocks[i][j] == 1) {
          int x = current.x + j;
          int y = current.y + (i - 1);
          var->field[y][x] = 1;
        }
      }
    }
  } else {
    res = 1;
  }
  return res;
}
int count_points(GameInfo_t *var) {
  int res = 0;
  int points[] = {0, 100, 300, 700, 1500};
  for (int i = HEIGHT_MAIN - 1; i >= 0; i--) {
    while (filled_line(i, var)) {
      move_upper_lines(i, var);
      res += 1;
    }
  }
  if (res > 4) {
    res = 4;
  }
  return points[res];
}
int filled_line(int i, GameInfo_t *var) {
  int res = 1;
  for (int j = 0; j < WIDTH_MAIN; j++) {
    if (var->field[i][j] == 0) {
      res = 0;
      break;
    }
  }
  return res;
}
int move_upper_lines(int i, GameInfo_t *var) {
  int res = 0;
  if (var) {
    if (i == 0) {
      for (int j = 0; j < WIDTH_MAIN; j++) {
        var->field[i][j] = 0;
      }
    } else {
      for (int k = i; k >= 1; k--) {
        for (int j = 0; j < WIDTH_MAIN; j++) {
          var->field[k][j] = var->field[k - 1][j];
        }
      }
    }
  } else {
    res = 1;
  }
  return res;
}
int update_score(GameInfo_t *var) {
  int res = 0;
  if (var) {
    var->high_score = var->score;
  } else {
    res = 1;
  }
  return res;
}
int create_new_figure(GameInfo_t *var, figure *current) {
  int res = 0;
  if (var && current) {
    init_current_figure(current, var->next);
    init_next_figure(var);
  } else {
    res = 1;
  }
  return res;
}
void free_figure(figure *var) {
  if (var) {
    for (int i = 0; i < VALUE_SAMPLE; i++) {
      free(var->blocks[i]);
    }
    free(var->blocks);
  }
}
int memory_allocation_figure(figure *var) {
  int res = 0;
  if (var) {
    var->blocks = malloc(sizeof(int *) * VALUE_SAMPLE);
    if (var->blocks) {
      for (int i = 0; i < VALUE_SAMPLE; i++) {
        var->blocks[i] = calloc(VALUE_SAMPLE, sizeof(int));
        if (var->blocks[i] == NULL) {
          res = 1;
          break;
        }
      }
    } else {
      res = 1;
    }
  } else {
    res = 1;
  }
  return res;
}
int erase_field(GameInfo_t *var, figure current) {
  int res = 0;
  if (var) {
    for (int i = 0; i < VALUE_SAMPLE; i++) {
      for (int j = 0; j < VALUE_SAMPLE; j++) {
        if (current.blocks[i][j] == 1) {
          int x = current.x + j;
          int y = current.y + (i - 1);
          var->field[y][x] = 0;
        }
      }
    }
  } else {
    res = 1;
  }
  return res;
}
void userInput(UserAction_t action, bool hold) {
  (void)hold;
  switch (action) {
    case Left:
      writening_current_figure(0, 0, 0, Left);
      break;
    case Right:
      writening_current_figure(0, 0, 0, Right);
      break;
    case Down:
      writening_current_figure(0, 0, 1, Down);
      break;
    case Pause:
      writening_current_figure(0, 0, 0, Pause);
      break;
    case Terminate:
      writening_current_figure(0, 0, 0, Terminate);
      break;
    case Action:
      writening_current_figure(0, 0, 0, Action);
      break;
    case Start:
      writening_current_figure(0, 0, 0, Start);
      break;
    default:
      break;
  }
}
void free_in_the_end_of_game(GameInfo_t *var, figure *current) {
  if (var) {
    var->pause = 3;
    free_field(var);
    free_next(var);
  }
  if (current) {
    free_figure(current);
  }
}
int rotate_figure(GameInfo_t *var, figure *current) {
  int res = 0;
  if (var && current) {
    figure tmp;
    tmp.x = current->x;
    tmp.y = current->y;
    memory_allocation_figure(&tmp);

    for (int i = 0; i < VALUE_SAMPLE; i++) {
      for (int j = 0; j < VALUE_SAMPLE; j++) {
        tmp.blocks[i][j] = current->blocks[j][VALUE_SAMPLE - 1 - i];
      }
    }
    if (!collision(*var, tmp) && square(current)) {
      for (int i = 0; i < VALUE_SAMPLE; i++) {
        for (int j = 0; j < VALUE_SAMPLE; j++) {
          current->blocks[i][j] = tmp.blocks[i][j];
        }
      }
    }
    free_figure(&tmp);
  } else {
    res = 1;
  }
  return res;
}
int clear_field(GameInfo_t *var) {
  int res = 0;
  if (var) {
    for (int i = 0; i < HEIGHT_MAIN; i++) {
      for (int j = 0; j < WIDTH_MAIN; j++) {
        var->field[i][j] = 0;
      }
    }
  } else {
    res = 1;
  }
  return res;
}
int writening_high_score(int score) {
  int res = 0;
  FILE *file = fopen("brick_game/tetris/high_score.txt", "r+");
  if (file) {
    fprintf(file, "%d", score);
    fclose(file);
  } else {
    res = 1;
  }
  return res;
}
int square(figure *figure) {
  int res = 0, k = 0, c = 1;
  if (!figure) {
    res = 1;
  } else {
    int square_coord[] = {1, 2, 1, 3, 2, 2, 2, 3};
    for (int i = 0; i < VALUE_SAMPLE; i++) {
      for (int j = 0; j < VALUE_SAMPLE; j++) {
        if (figure->blocks[i][j] == 1 && i == square_coord[k] &&
            j == square_coord[c]) {
          k += 2;
          c += 2;
        } else if (figure->blocks[i][j] == 1 &&
                   (i != square_coord[k] || j != square_coord[c])) {
          res = 2;
          break;
        }
      }
    }
  }
  return res;
}
GameInfo_t do_action(GameInfo_t *var, figure *current, UserAction_t action,
                     int check_high_score) {
  switch (action) {
    case Left:
      erase_field(var, *current);
      move_left(current);
      if (collision(*var, *current)) {
        move_right(current);
      }
      put_current(var, *current);
      break;
    case Right:
      erase_field(var, *current);
      move_right(current);
      if (collision(*var, *current)) {
        move_left(current);
      }
      put_current(var, *current);
      break;
    case Pause:
      if (var->pause == 0) {
        var->pause = 1;
      } else {
        var->pause = 0;
      }
      break;
    case Terminate:
      if (var->score > check_high_score) {
        writening_high_score(var->score);
      }
      free_in_the_end_of_game(var, current);
      break;
    case Action:
      erase_field(var, *current);
      rotate_figure(var, current);
      put_current(var, *current);
      break;
    case Start:
      clear_field(var);
      var->score = 0;
      create_new_figure(var, current);
      put_current(var, *current);
      break;
    default:
      break;
  }
  return *var;
}
