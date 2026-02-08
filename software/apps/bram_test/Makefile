ALL_EXE = test
all: $(ALL_EXE) 

CC=gcc
CFLAGS=-I. -I../include  -I../API -I../hw -Wall

DEPS = Makefile ../hw/proto2_hw.h

PUBLIC_LIBS = ../API/mem-io.o
LIB_FILES =

LIB_OBJS := $(subst .c,.o,$(LIB_FILES))

test: test.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -g -o $@ $^

run: test 
	@echo "You must be in root."
	./test 

clean:
	rm -rf *.o $(ALL_EXE) 

#pattern rules
%.o: %.c $(DEPS)
# $<: first prerequisite
	$(CC) -g -c -o $@ $< $(CFLAGS)
