cflags=gcc -Wall -Wextra -Werror -o
.PHONY: all s21_cat clean

all: s21_cat test

s21_cat:
	$(cflags) s21_cat main.c
test:
	chmod u+x ./t_cat.sh
	./t_cat.sh

style:
	clang-format -style=google -n *.c

format:
	clang-format -style=google -i *.c
