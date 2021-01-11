CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -I./longarythmetic/headers/

FUNC_C = ./longarythmetic/src/*.c
FUNC_O = ./longarythmetic/src/*.o

OBJ = $(patsubst %.c, %.o, $(wildcard $(FUNC_C)))
OBJ_MAIN = $(patsubst %.c, %.o, $(wildcard *.c))

app.exe: $(OBJ) $(OBJ_MAIN)
	$(CC) -o $@ $^
	rm *.o $(FUNC_O)

run:
	./app.exe

clean:
	rm *.exe $(FUNC_O)
