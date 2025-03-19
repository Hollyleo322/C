#pragma once

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH_MAIN 10
#define HEIGHT_MAIN 20
#define COUNT_FIGURES 7
#define VALUE_SAMPLE 5

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct figure {
  int x;
  int y;
  int **blocks;
} figure;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

void main_game();
void main_menu();
int init_object(GameInfo_t *var, FILE *file);
int memory_allocation_field(GameInfo_t *var);
void free_field(GameInfo_t *var);
void free_next(GameInfo_t *var);
int read_high_score(int *high_score, FILE *file);
int init_ints(GameInfo_t *var);
int init_next_figure(GameInfo_t *var);
int memory_allocation_next(GameInfo_t *var);
int init_current_figure(figure *current, int **ptr_next);
int window_of_game(int period, int new);
GameInfo_t writening_current_figure(int new_current, int new_game, int update,
                                    UserAction_t action);
int move_down(figure *var);
int collision(GameInfo_t var, figure current);
int move_down(figure *var);
int move_up(figure *var);
int put_current(GameInfo_t *var, figure current);
int count_points(GameInfo_t *var);
int move_upper_lines(int i, GameInfo_t *var);
int update_score(GameInfo_t *var);
int create_new_figure(GameInfo_t *var, figure *current);
int filled_line(int i, GameInfo_t *var);
void free_figure(figure *var);
int memory_allocation_figure(figure *var);
int erase_field(GameInfo_t *var, figure current);
void moving_figure(WINDOW *var_window_of_game);
int move_right(figure *var);
int move_left(figure *var);
void output_pause_window();
void free_in_the_end_of_game(GameInfo_t *var, figure *current);
int rotate_figure(GameInfo_t *var, figure *current);
void output_additional_window(WINDOW *win, GameInfo_t *var);
int get_start_for_add(GameInfo_t *var);
void output_next_figure(WINDOW *win, GameInfo_t *var);
int clear_field(GameInfo_t *var);
int writening_high_score(int score);
int square(figure *figure);
void init_field(GameInfo_t *var, int number);
GameInfo_t do_action(GameInfo_t *var, figure *current, UserAction_t action,
                     int check_high_score);
void output_control_window(int start_x, int start_y);