# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for game

# Definitions.
CC = g++
CFLAGS = 
DEL = rm


# Default target.
all: source.out


# Compile: create object files from C source files.
source.o: source.c 
	$(CC) -c $(CFLAGS) $< -o $@




# Link: create ELF output file from object files.
source.out: source.o .o
	$(CC) $(CFLAGS) $^ -o $@ -lm


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


