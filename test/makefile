CC = gcc
CFLAGS = -O0 -g -Wall -Wextra -Wpedantic -I..
LDFLAGS = -lm -pthread -lz -lcmocka
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all -q
CODE = $(wildcard *_test.c)
OBJS = $(CODE:_test.c=)
VALS = $(OBJS:=_val)
TEST = $(OBJS:=_test)

.PHONY: clean
.SILENT: clean

all: $(TEST)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

../%.o:	../%c
	cd ..
	make $@

run: all
	for NAME in $(TEST); do \
		./$$NAME; \
	done

val: $(VALS)

%_val: %_test
	$(VALGRIND) ./$<

%_test: ../%.o %_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(TEST) $(VALS)

#Special cases
database_test: ../csv.o ../errors.o ../record.o ../array.o ../database.o database_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
