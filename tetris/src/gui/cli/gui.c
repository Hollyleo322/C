#include "../../brick_game/tetris/s21_tetris.h"
void main_menu() {
  char *array_main[] = {
      "start game",
      "control buttons",
      "exit game",
      "s21_tetris",
      "press enter to approve a choice",
      "--->",
  };
  int height_menu[] = {8, 11, 14};
  int x_menu[] = {14, 12, 14};
  int pointer = 0, target, out = 0;
  int start_x = (COLS - 40) / 2;
  int start_y = (LINES - 20) / 2;
  WINDOW *main_menu_var = newwin(20, 40, start_y, start_x);
  keypad(main_menu_var, true);
  while (1) {
    box(main_menu_var, 0, 0);
    wattron(main_menu_var, A_BOLD);
    mvwprintw(main_menu_var, 1, 15, "%s", array_main[3]);
    mvwprintw(main_menu_var, 2, 5, "%s", array_main[4]);
    wattroff(main_menu_var, A_BOLD);
    for (int i = 0; i < 3; i++) {
      if (i == pointer) {
        mvwprintw(main_menu_var, height_menu[i], 3, "%s", array_main[5]);
        wattron(main_menu_var, A_STANDOUT);
      }
      mvwprintw(main_menu_var, height_menu[i], x_menu[i], "%s", array_main[i]);
      wattroff(main_menu_var, A_STANDOUT);
    }
    target = wgetch(main_menu_var);
    werase(main_menu_var);
    switch (target) {
      case KEY_UP:
        pointer -= 1;
        if (pointer == -1) {
          pointer = 0;
        }
        break;
      case KEY_DOWN:
        pointer += 1;
        if (pointer == 3) {
          pointer = 2;
        }
        break;
      case '\n':
        if (!pointer) {
          werase(main_menu_var);
          wrefresh(main_menu_var);
          delwin(main_menu_var);
          main_game();
          out = 1;
        } else if (pointer == 1) {
          werase(main_menu_var);
          wrefresh(main_menu_var);
          output_control_window(start_x, start_y);
        } else {
          out = 1;
        }
        break;
    }
    if (out) {
      break;
    }
  }
}
int window_of_game(int period, int new) {
  int res = 0;
  static WINDOW *var_window_of_game, *additional_window;
  if (new) {
    int start_x = (COLS - 12) / 2;
    int start_y = (LINES - 22) / 2;
    var_window_of_game = newwin(22, 12, start_y, start_x);
    additional_window = newwin(22, 12, start_y, start_x + 13);
    keypad(var_window_of_game, true);
    nodelay(var_window_of_game, true);
  }
  box(var_window_of_game, 0, 0);
  box(additional_window, 0, 0);
  GameInfo_t var;
  if (!period) {
    var = updateCurrentState();
  } else {
    var = writening_current_figure(0, 0, 0, 5);
  }
  if (var.pause == 4) {
    res = 2;
  }
  if (var.pause == 3 || var.pause == 2) {
    res = 1;
    werase(var_window_of_game);
    delwin(var_window_of_game);
    werase(additional_window);
    delwin(additional_window);
  } else {
    for (int i = 0; i < HEIGHT_MAIN; i++) {
      for (int j = 0; j < WIDTH_MAIN; j++) {
        if (var.field[i][j] == 1) {
          wattron(var_window_of_game, A_STANDOUT);
          mvwaddch(var_window_of_game, i + 1, j + 1, ' ');
          wattroff(var_window_of_game, A_STANDOUT);
        } else {
          mvwprintw(var_window_of_game, i + 1, j + 1, "%s", ".");
        }
      }
    }
    output_additional_window(additional_window, &var);
    moving_figure(var_window_of_game);
  }
  if (var.pause == 1) {
    output_pause_window(var_window_of_game);
  }
  return res;
}
void moving_figure(WINDOW *var_window_of_game) {
  int step = wgetch(var_window_of_game);
  werase(var_window_of_game);
  switch (step) {
    case KEY_LEFT:
      userInput(Left, false);
      break;
    case KEY_RIGHT:
      userInput(Right, false);
      break;
    case KEY_UP:
      break;
    case KEY_DOWN:
      userInput(Down, true);
      break;
    case '\n':
      userInput(Pause, false);
      break;
    case 'Q':
    case 'q':
      userInput(Terminate, false);
      break;
    case ' ':
      userInput(Action, false);
      break;
    case 'S':
    case 's':
      userInput(Start, false);
      break;
  }
}
void output_pause_window() {
  int start_x = (COLS - 12) / 2 + 3;
  int start_y = (LINES - 22) / 2 + 6;
  WINDOW *pause_window = newwin(3, 7, start_y, start_x);
  box(pause_window, 0, 0);
  mvwprintw(pause_window, 1, 1, "%s", "Pause");
  while (1) {
    int check = wgetch(pause_window);
    if (check == '\n') {
      werase(pause_window);
      wrefresh(pause_window);
      delwin(pause_window);
      writening_current_figure(0, 0, 0, Pause);
      break;
    }
  }
}
void output_additional_window(WINDOW *win, GameInfo_t *var) {
  if (var && win) {
    mvwprintw(win, 1, 4, "%s", "NEXT");
    output_next_figure(win, var);
    mvwprintw(win, 6, 3, "%s", "SCORE");
    char array_score[10], array_high_score[10], array_lvl[10];
    sprintf(array_score, "%06d", var->score);
    sprintf(array_high_score, "%06d", var->high_score);
    sprintf(array_lvl, "%d", var->level);
    mvwprintw(win, 8, 3, "%s", array_score);
    mvwprintw(win, 10, 1, "%s", "HIGH SCORE");
    mvwprintw(win, 12, 3, "%s", array_high_score);
    mvwprintw(win, 14, 3, "%s", "LEVEL");
    mvwprintw(win, 16, 5, "%s", array_lvl);
    wrefresh(win);
    werase(win);
  }
}
int get_start_for_add(GameInfo_t *var) {
  int res = 0, check = 0;
  for (int i = 0; !check; i++) {
    for (int j = 0; j < VALUE_SAMPLE; j++) {
      if (var->next[i][j] == 1) {
        check = j;
        break;
      }
    }
  }
  switch (check) {
    case 1:
      res = 0;
      break;
    case 2:
      res = 1;
      break;
    case 3:
      res = 2;
      break;
  }
  return res;
}
void output_next_figure(WINDOW *win, GameInfo_t *var) {
  if (win && var) {
    int flag_change_of_row = 0, number_of_col = 0, check_i = 0;
    int k = 3, s = 4;
    int add = get_start_for_add(var);
    for (int i = 0; i < VALUE_SAMPLE; i++) {
      if (flag_change_of_row) {
        k += 1;
        s = 3;
      }
      for (int j = 0; j < VALUE_SAMPLE; j++) {
        if (var->next[i][j] == 1) {
          if (!flag_change_of_row) {
            check_i = i;
            number_of_col = j;
          }
          if (check_i != i) {
            if (j == number_of_col) {
              s++;
            } else if (j > number_of_col) {
              s += 2;
            }
            if (j + 2 == number_of_col) {
              s -= 1;
            }
            check_i = i;
          }
          wattron(win, A_STANDOUT);
          mvwaddch(win, k, s + add, ' ');
          wattroff(win, A_STANDOUT);
          flag_change_of_row = 1;
          s++;
        }
      }
    }
  }
}
void output_control_window(int start_x, int start_y) {
  char *array_input[] = {"s - restart",
                         "SPACE - rotate",
                         "q - exit from game",
                         "ENTER - pause",
                         "< > v ^ arrows key - move figure",
                         "press any key to exit"};
  WINDOW *control_window = newwin(20, 40, start_y, start_x);
  keypad(control_window, true);
  box(control_window, 0, 0);
  wattron(control_window, A_BOLD);
  mvwprintw(control_window, 1, 2, "%s", array_input[0]);
  mvwprintw(control_window, 3, 2, "%s", array_input[1]);
  mvwprintw(control_window, 5, 2, "%s", array_input[2]);
  mvwprintw(control_window, 7, 2, "%s", array_input[3]);
  mvwprintw(control_window, 9, 2, "%s", array_input[4]);
  wattroff(control_window, A_BOLD);
  mvwprintw(control_window, 18, 1, "%s", array_input[5]);
  int target = wgetch(control_window);
  switch (target) {
    case '\n':
      werase(control_window);
      delwin(control_window);
      break;
  }
  return;
}