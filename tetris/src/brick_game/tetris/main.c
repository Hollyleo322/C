#include "s21_tetris.h"

int main(void) {
  int res = 0;
  if (freopen("/dev/tty", "r", stdin)) {
    initscr();              // Иницилизация ncurses
    nodelay(stdscr, true);  // снимает блокирование getch
    curs_set(0);  // убирает отображение курсора;
    cbreak();
    noecho();  // убирает отображение ввода
    main_menu();
    endwin();  // закрытие ncurses
  } else {
    res = 1;
    printf("ERROR of freopen!!!\n");
  }
  return res;
}