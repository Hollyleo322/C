cflags=gcc -Wall -Wextra -Werror -std=c11 -o
gcov=--coverage
.PHONY : test test1 s21_decimal.a gcov_report 

all : s21_decimal.a test1 gcov_report

test1:
	$(cflags) test1 test1.c s21_decimal.c -lcheck -lsubunit -g -lm
	./test1
s21_decimal.a : s21_decimal.o
	ar rcs s21_decimal.a s21_decimal.o
	rm -r *.o
s21_decimal.o : 
	gcc -c s21_decimal.c -o s21_decimal.o
clean:
	rm -rf *.o *.a *.gcda *.gcno *.info tests 
	rm -rf test_gcov test1 
	rm -rf ./report
	rm -rf ./-o-s21_decimal.gcno ./-o-test1.gcno

gcov_report:
	$(cflags) test_gcov $(gcov) test1.c s21_decimal.c -lcheck -lsubunit -lm
	./test_gcov
	lcov -t "name" -o s21_decimal.info -c -d .
	genhtml -o report s21_decimal.info
style:
	clang-format -style=google -n *.c *.h
format:
	clang-format -style=google -i *.c *.h

