libs = -lstdc++ -lSDL2main -lSDL2
prms = -O0 -Wall -Wextra -g -m64

SRCS = src/*.cpp
INCS = -I./include/
OUTDIR = ./bin/debug/main

CC = clang++

.PHONY all: compile run

compile:
	@${CC} ${SRCS} ${INCS} ${prms} ${libs} -o ${OUTDIR}

run:
	@${OUTDIR}