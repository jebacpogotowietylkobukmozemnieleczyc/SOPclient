CC=gcc
CFLAGS=-Wall -I.
DEPS = inf117244_kdata.h
OBJ = inf117244_k.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

inf117244_k: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)