CFLAGS=-g -Wall -std=c99
GCC=gcc
RM=rm -f

# all .c files in src and one hierachy beneath
TESTS=$(wildcard src/*.c src/**/*c)

all:
    $(GCC) $(CFLAGS) $(TESTS)

clean:  $(OBJECTS) $(TESTS)
