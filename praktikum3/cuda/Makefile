CC ?= gcc
SIZE ?= 1024

.PHONY: cuda
cuda: wrapper.cpp default.cu ../util.c
	nvcc -DSIZE=$(SIZE) $^ -allow-unsupported-compiler -o cuda

.PHONY: test
test: test.cpp default.cu ../util.c
	nvcc -DSIZE=$(SIZE) $^ -allow-unsupported-compiler -fmad=false -o test
	./test

.PHONY: clean
clean:
	rm -f cuda test *.o
