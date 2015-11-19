NAME = cpareia
CC = gcc
CFLAGS = -O0 -g -Wall -Wextra -pedantic -fdiagnostics-color=always `pkg-config --cflags libxml-2.0 glib-2.0` -DSINGLE_BLOCKER
#CFLAGS = -O3 -march=native -Wall -Wextra -pedantic -fdiagnostics-color=always `pkg-config --cflags libxml-2.0 glib-2.0` -DSINGLE_BLOCKER
LDFLAGS = `pkg-config --libs libxml-2.0 glib-2.0` -lm
SRC = .
DEPS = $(wildcard $(SRC)/*.h)
CODE = $(wildcard $(SRC)/*.c)
OBJ = $(patsubst %.c,%.o,$(CODE))
PROJ = input/project.xml

.PHONY: install clean dist-gzip dist-bzip2 dist-xz dist
.SILENT: install clean dist-gzip dist-bzip2 dist-xz dist

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(NAME)
	./$(NAME) $(PROJ)

val: $(NAME)
	valgrind --suppressions=glib.supp --leak-check=full ./$(NAME) $(PROJ)

clean:
	rm -f $(SRC)/*.o *~ $(SRC)/*~ $(NAME)
