cflags=gcc -Wall -Wextra -Werror -std=c11 -o

.PHONY: test
all: install gcov_report

install:
	gcc -c brick_game/tetris/s21_tetris.c -o brick_game/tetris/s21_tetris.o
	ar rcs brick_game/tetris/s21_tetris.a brick_game/tetris/s21_tetris.o
	rm -rf brick_game/tetris/*.o
	$(cflags) game brick_game/tetris/main.c gui/cli/gui.c -g -lncurses ./brick_game/tetris/s21_tetris.a
uninstall:
	rm -rf brick_game/tetris/s21_tetris.a
	rm -rf game
clean:
	rm -rf *.gcda *.gcno *.info test_gcov test report s21_tetris
dvi:
	makeinfo --dvi manual.texi
html:
	makeinfo --html manual.texi
dist:
	tar -cf s21_tetris.tar brick_game/tetris/main.c  brick_game/tetris/s21_tetris.c brick_game/tetris/s21_tetris.h brick_game/tetris/high_score.txt gui/cli/gui.c

test:
	$(cflags) test brick_game/tetris/s21_tetris.c brick_game/tetris/test_tetris.c gui/cli/gui.c brick_game/tetris/test.c -lncurses -lcheck -lsubunit -lm
	./test

gcov_report:
	$(cflags) test_gcov --coverage brick_game/tetris/s21_tetris.c brick_game/tetris/test_tetris.c gui/cli/gui.c brick_game/tetris/test.c -lncurses -lcheck -lsubunit -lm
	./test_gcov
	lcov -t "name" -o s21_tetris.info -c -d .
	genhtml -o report s21_tetris.info
style:
	clang-format -style=google -n brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c

format:
	clang-format -style=google -i brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c