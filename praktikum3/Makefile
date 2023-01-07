
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
