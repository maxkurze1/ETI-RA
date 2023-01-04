SIZE = 1024 2048 4096
NTASKS = 1 2 4 6 8 12 16 20 23 24 25 28 32 36 40 48
CFLAGS = -Wall -Wextra -O3 -march=native
SRC = mpi.c
CC = mpicc
RUN ?= srun
all = $(foreach S,$(SIZE),$(addprefix timing/mpi-$S-,$(NTASKS)))
allbin = $(addprefix bin/mpi-,$(SIZE))

.PHONY: all
all: $(all)

# mpi-$SIZE
bin/mpi-%: | bin
	$(CC) -DSIZE="$(word 1, $(subst -, ,$(*F)))" $(CFLAGS) $(SRC) -o $@

bin:
	mkdir bin
timing:
	mkdir timing

.PHONY: clean
clean:
	rm -r bin timing

# mpi-$SIZE-$ntasks
.SECONDEXPANSION:
timing/mpi-%: bin/mpi-$$(word 1,$$(subst -, ,$$(*F))) | timing
	echo 'values' > $@
	for i in {0..9}; do \
		$(RUN) --ntasks $(word 2,$(subst -, ,$(*F))) --exclusive $< >> $@; \
	done

