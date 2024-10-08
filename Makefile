all:
	gcc -g -o main.o -Wall -Werror -Wextra -pedantic main.c -lraylib -lm
