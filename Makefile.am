AUTOMAKE_OPTIONS = foreign
SUBDIRS = src
#NAME = cpareia
#CC = gcc
#CFLAGS = -Wall -Wextra -Wpedantic
#CFLAGS += `pkg-config --cflags libxml-2.0`
#LDFLAGS = `pkg-config --libs libxml-2.0` -lm -pthread -lz
#SRC = .
#DEPS = $(wildcard $(SRC)/*.h)
#CODE = $(wildcard $(SRC)/*.c)
#OBJ = $(patsubst %.c,%.o,$(CODE))
#PROJ = input/project.xml
#OPT = -O0 -g
#OPT = -O3 -march=native -flto
#
#.PHONY: install clean dist-gzip dist-bzip2 dist-xz dist
#.SILENT: install clean dist-gzip dist-bzip2 dist-xz dist
#
#%.o: %.c %.h
#	$(CC) $(CFLAGS) $(OPT) -c -o $@ $<
#
#$(NAME): $(OBJ)
#	$(CC) $(CFLAGS) $(OPT) -o $@ $^ $(LDFLAGS)
#
#run: $(NAME)
#	./$(NAME) -p $(PROJ)
#
#val: $(NAME)
#	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) -p $(PROJ)
#
#clean:
#	rm -f $(SRC)/*.o *~ $(SRC)/*~ $(NAME)
