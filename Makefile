CFLAGS = -pedantic -Wall -Wextra -std=c99 -g
SOURCES = $(filter-out parser_test.c expr_parser_test.c, $(wildcard *.c))
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = ifj parser_test expr_parser_test

all: expr_parser_test ifj parser_test

parser_test: parser_test.c expr_parser.c instrlist.c
	gcc $(CFLAGS) $^ -o $@

expr_parser_test: expr_parser_test.c expr_parser.c instrlist.c
	gcc $(CFLAGS) -DDEBUG $^ -o $@

ifj: $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

clean:
	rm *.o $(PROGRAMS)
