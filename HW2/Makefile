CC = gcc
CFLAGS = -Wall -fPIC
STATIC_OBJS = fred.o john.o
SHARED_OBJS = bill.o sam.o

all: program

fred.o: fred.c
	$(CC) -c fred.c

john.o: john.c
	$(CC) -c john.c

bill.o: bill.c
	$(CC) $(CFLAGS) -c bill.c

sam.o: sam.c
	$(CC) $(CFLAGS) -c sam.c

libstatic.a: $(STATIC_OBJS)
	ar crv libstatic.a $(STATIC_OBJS)

libshared.so: $(SHARED_OBJS)
	$(CC) -shared -o libshared.so $(SHARED_OBJS) -lm

interpose_static.o: interpose_static.c
	$(CC) -c interpose_static.c

libinterpose.so: interpose_dynamic.c
	$(CC) $(CFLAGS) -shared -o libinterpose.so interpose_dynamic.c

program.o: program.c
	$(CC) -c program.c

program: program.o interpose_static.o libshared.so libstatic.a
	$(CC) -o program program.o interpose_static.o -L. -lstatic -lshared -lm

run: libinterpose.so program
	DYLD_INSERT_LIBRARIES=./libinterpose.so DYLD_FORCE_FLAT_NAMESPACE=1 ./program



clean:
	rm -f *.o *.a *.so program
