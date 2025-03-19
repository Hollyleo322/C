cflags=gcc -Wall -Wextra -Werror -o
.PHONY: all s21_grep clean

all: s21_grep test test_800_cases

s21_grep:
	$(cflags) s21_grep s21_grep.c -lpcre -g
test:
	chmod u+x t_grep_2var.sh
	./t_grep_2var.sh
test_800_cases:
	chmod u+x t_grep.sh
	./t_grep.sh

style:
	clang-format -style=google -n *.c

format:
	clang-format -style=google -i *.c
pcre_lib:
	sudo apt-get install libpcre3 libpcre3-dev