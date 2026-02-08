ALL_EXE = mmap-fpga
all: $(ALL_EXE) 

CC=gcc
CFLAGS=-I. -I../include  -I../API -I../hw

DEPS = Makefile

PUBLIC_LIBS = ../API/utils.o ../API/mem-io.o
LIB_FILES =

LIB_OBJS := $(subst .c,.o,$(LIB_FILES))

mmap-fpga: mmap-fpga.o $(LIB_OBJS) $(PUBLIC_LIBS)
# $@: target file name, $^: all prerequisites
	gcc -g -o $@ $^

run: mmap-fpga
	@echo "You must be in root."
	./mmap-fpga f0000000

clean:
	rm -rf *.o $(ALL_EXE) 

#pattern rules
%.o: %.c $(DEPS)
# $<: first prerequisite
	$(CC) -g -c -o $@ $< $(CFLAGS)
