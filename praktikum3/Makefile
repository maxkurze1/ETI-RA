SIZES ?= 512 1024 2048 2436 2896 3444 4096 4870 5792 6888 8192 9742 11586 13778 16384

.PHONY: openmp cuda clean
openmp:
	$(MAKE) -C openmp openmp
	./openmp/openmp

cuda:
	$(MAKE) -C cuda cuda
	./cuda/cuda

clean:
	$(MAKE) -C cuda clean
	$(MAKE) -C openmp clean

profile:
	SIZES="$(SIZES)" $(MAKE) -C cuda profile
	SIZES="$(SIZES)" $(MAKE) -C openmp profile
