PUBLIC_LIB = utils.o mem-io.o
all: $(PUBLIC_LIB) 

CC=gcc
CFLAGS=-I. -I../include
#DEPS = rf_proto_hw.h $(subst .o,.h,$(PUBLIC_LIB )) 
DEPS = $(subst .o,.h,$(PUBLIC_LIB )) 

utils.o: utils.c $(DEPS)
# $@: target file name, $^: all prerequisites
	gcc -c  $^

mem-io.o: mem-io.c $(DEPS)
# $@: target file name, $^: all prerequisites
clean:
	rm -rf *.o *.gch

#pattern rules
%.o: %.c $(DEPS)
# $<: first prerequisite
	$(CC) -g -c -o $@ $< $(CFLAGS)
