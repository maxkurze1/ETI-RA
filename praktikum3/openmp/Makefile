CC = nvc
SIZE ?= 1024
OFLAGS ?= -O3 -mp=gpu
CFLAGS ?= -Wall -Wextra

openmp: default.c wrapper.c ../util.c
	$(CC) -DSIZE=$(SIZE) $^ $(CFLAGS) $(OFLAGS) -o $@ 

.PHONY: clean
clean:
	rm -f *.o openmp test

.PHONY: test
test: default.c testing.c ../util.c
	$(CC) -DSIZE=$(SIZE) $^ $(CFLAGS) $(OFLAGS) -o $@ && ./test

