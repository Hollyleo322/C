cflags=gcc -Wall -Wextra -Werror -std=c11 -o
.PHONY : all s21_string test s21_string.a test1 clean test2 test3 test4 test5
gcov=--coverage
all: s21_string.a gcov_report
test: test1 test2 test3 test4 test5

clean: delete_report
style:
	clang-format -style=google -n *.c *.h
format:
	clang-format -style=google -i *.c *.h
s21_string.a:
	rm -rf *.a
	gcc -c s21_string.c -o s21_string.o
	gcc -c s21_sprintf.c -o s21_sprintf.o
	gcc -c s21_sscanf.c -o s21_sscanf.o
	gcc -c s21_csharp.c -o s21_csharp.o
	ar q s21_string.a s21_string.o
	ar q s21_string.a s21_sprintf.o
	ar q s21_string.a s21_sscanf.o
	ar q s21_string.a s21_csharp.o
	rm -r *.o
gcov_report:
	$(cflags) test1_gcov $(gcov) test1.c s21_string.c s21_sprintf.c -lcheck -lsubunit -lm
	./test1_gcov
	$(cflags) test2_gcov $(gcov) test2.c s21_sprintf.c s21_string.c -lcheck -lsubunit -lm
	./test2_gcov
	$(cflags) test3_gcov $(gcov) test3.c s21_sprintf.c s21_string.c -lcheck -lsubunit -lm
	./test3_gcov
	$(cflags) test4_gcov $(gcov) test4.c s21_sscanf.c s21_string.c s21_sprintf.c s21_csharp.c -lcheck -lsubunit -lm
	./test4_gcov
	$(cflags) test5_gcov $(gcov) test5.c s21_csharp.c s21_string.c s21_sprintf.c -lcheck -lsubunit -lm
	./test5_gcov
	lcov -t "name" -o s21_string.info -c -d .
	genhtml -o report s21_string.info
delete_report:
	rm -rf report test1 test2 test3 test4 test5 test1_gcov test2_gcov test3_gcov test4_gcov test5_gcov *.gcda *.gcno *.info
test1:
	$(cflags) test1 test1.c ./s21_string.a -g -lcheck -lsubunit -lm
	./test1
test2:
	$(cflags) test2 test2.c ./s21_string.a -lcheck -lsubunit -lm
	./test2
test3:
	$(cflags) test3 test3.c ./s21_string.a -lcheck -lsubunit -lm
	./test3
test4:
	$(cflags) test4 test4.c ./s21_string.a -lcheck -lsubunit -lm
	./test4
test5:
	$(cflags) test5 test5.c ./s21_string.a -lcheck -lsubunit -lm
	./test5
lcov:
	sudo apt install lcov