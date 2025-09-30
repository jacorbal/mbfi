# vim: set filetype=make nowrap noexpandtab tabstop=4:
# Makefile
#

## Directories
PWD   = $(CURDIR)
I_DIR = ${PWD}/include
S_DIR = ${PWD}/src
L_DIR = ${PWD}/lib
O_DIR = ${PWD}/obj
B_DIR = ${PWD}/bin


## Compiler & linker opts.
CC           = gcc
CCSTANDARD   = c90 #{c{90,99,03,11,0x,14,1z},gnu{90,99,03,11,0x,14,1z}}
OPTIMIZATION = 3 #0:debug; 1:optimize; 2:optimize even more; 3:optimize yet more
WARNINGS     = -Werror -Wall -Wshadow -Wextra -Wwrite-strings #-Wconversion
EXTRA_OPTS   = -fdiagnostics-color=always
CCFLAGS      = --pedantic ${WARNINGS} -std=${CCSTANDARD} ${EXTRA_OPTS} -I ${I_DIR}
LDFLAGS      = -l m ${SDLFLAGS} -L ${L_DIR}


# Use `make DEBUG=1` to add debugging information, symbol table, etc.
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CCFLAGS += -DDEBUG -g -ggdb -O0
else
	CCFLAGS += -DNDEBUG -O${OPTIMIZATION}
endif


## Makefile opts.
SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .h .c .o


## Files options
TARGET = ${B_DIR}/main
OBJS = $(patsubst ${S_DIR}/%.c, ${O_DIR}/%.o, $(wildcard ${S_DIR}/*.c))
ARGS ?=

## Linkage
${TARGET}: ${OBJS}
	${CC} -o $@ $^ ${LDFLAGS}


## Compilation
${O_DIR}/%.o: ${S_DIR}/%.c
	${CC} ${CCFLAGS} -c -o $@ $<


## Make options
.PHONY: all clean clean-obj clean-all run hard-run help

all:
	make ${TARGET}

clean-obj:
	rm --force ${OBJS}

clean-bin:
	rm --force ${TARGET}

clean:
	@make clean-obj
	@make clean-bin

clean-all:
	@make clean

run:
	${TARGET} ${ARGS}

hard:
	@make clean
	@make all

hard-run:
	@make clean
	@make all
	@make run

help:
	@echo "Type:"
	@echo "  'make all'......................... Build project"
	@echo "  'make run'................ Run binary (if exists)"
	@echo "  'make clean-obj'.............. Clean object files"
	@echo "  'make clean'....... Clean binary and object files"
	@echo "  'make hard'...................... Clean and build"
	@echo ""
	@echo " Binary will be placed in '${TARGET}'"

