SIZES ?= 512 1024 2048

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
