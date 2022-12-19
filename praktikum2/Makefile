
matrix-mult:
	mpicc -DSIZE="1024" matrix-mult.c -Wall -Wextra -o matrix-mult

run: matrix-mult
	mpirun -np 2 ./$^

.PHONY: clean
clean:
	if [ -f matrix-mult ]; then rm matrix-mult; fi
