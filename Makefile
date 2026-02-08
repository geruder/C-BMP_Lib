CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

LIB_NAME = libbmap.a

SRC = src/bmap.c
OBJ = bmap.o

all: $(LIB_NAME)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME): $(OBJ)
	ar rcs $@ $^

clean:
	rm -f *.o *.a test_app.exe test_app

test: all
	$(CC) $(CFLAGS) test_main.c -L. -lbmap -o test_app
	./test_app