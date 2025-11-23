all:
	./makeDirectory.sh
	gcc -Wall -Wextra -Werror main.c polish_notation.c stack.c double_stack.c graph.c validation.c -o build/graph -lm
format:
	clang-format -i --style=google *.c
style:
	clang-format -n --style=google *.c