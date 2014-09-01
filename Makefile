CFLAGS = -g

all: libftf.a api_test

OBJ=ftf_vector.o ftf_cmd.o ftf_vty.o ftf_util.o ftf_io.o ftf_resolve.o ftf.o

libftf.so:$(OBJ)
	gcc -g -fPIC -shared -o libftf.so $(OBJ)

libftf.a:$(OBJ)
	ar rcs libftf.a $(OBJ)

api_test: api_test.c
	gcc $(CFLAGS) -o $@ $^ ./libftf.a

install:
	cp -v libftf.a ~/usr/lib/
	cp -v ftf.h ~/usr/include/
	
clean:
	-rm -f *.o
	-rm -f libftf.so libftf.a api_test
