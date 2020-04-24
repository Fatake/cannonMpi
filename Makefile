CC = gcc
BMPIDIR = bin
UDIR = util

_DEPS = matrix.h
DEPS = $(patsubst %, $(UDIR)/%, $(_DEPS))

all: cannonmpi

cannonmpi: cannon.c $(DEPS)
	mpicc $^ -o $(BMPIDIR)/cannonmpi -lm 

clean:
	rm $(BMPIDIR)/*
