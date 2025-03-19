CFLAGS=gcc -Wall -Wextra -Werror -std=c11 -o

.PHONY: test gcov_report s21_matrix.a test1

all: s21_matrix.a gcov_report

s21_matrix.a:
	gcc -c s21_matrix.c -o s21_matrix.o
	ar rcs s21_matrix.a s21_matrix.o
	rm -rf *.o
test:
	$(CFLAGS) test test.c s21_matrix.c -lcheck -lsubunit -g -lm
	./test
gcov_report:
	$(CFLAGS) test_gcov --coverage test.c s21_matrix.c -lcheck -lsubunit -g -lm
	./test_gcov
	lcov -t "name" -o s21_decimal.info -c -d .
	genhtml -o report s21_decimal.info

clean:
	rm -rf *.o *.a *.gcda *.gcno *.info tests 
	rm -rf test_gcov test1 
	rm -rf ./report
	rm -rf ./-o-s21_decimal.gcno ./-o-test1.gcno ./test
	
style:
	clang-format -style=google -n *.c *.h
format:
	clang-format -style=google -i *.c *.h

