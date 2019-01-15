#makefile for lab1
CC=gcc
CFLAGS =-g -Wall

all: build

build: main.c
	$(CC) $(CFLAGS) -o count main.c

clean:
	rm search
