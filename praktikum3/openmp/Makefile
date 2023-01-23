CC = nvc
SIZES ?= 512 1024 2048 2436 2896 3444 4096 4870 5792 6888 8192
TEST_SIZE = 1735 
OFLAGS ?= -O3 -mp=gpu
CFLAGS ?= -Wall -Wextra
PROFILE_DIR = prof
LAUNCH_COUNT ?= 5

openmp-%: default.c wrapper.c ../util.c
	$(CC) -DSIZE=$* $^ $(CFLAGS) $(OFLAGS) -o $@ 

.PHONY: test
test: default.c testing.c ../util.c
	$(CC) -DSIZE=$(TEST_SIZE) $^ $(CFLAGS) -Mnofma -mp=gpu -o $@ && ./test

.PHONY: clean
clean:
	rm -f *.o openmp-* test
	rm -rf $(PROFILE_DIR)

$(PROFILE_DIR):
	mkdir $(PROFILE_DIR)

.PHONY: profile
profile: $(addprefix $(PROFILE_DIR)/profile-,$(SIZES)) | $(PROFILE_DIR)
	../convert.py $^ $(PROFILE_DIR)/profile.csv

$(PROFILE_DIR)/profile-%: openmp-% | $(PROFILE_DIR)
	ncu --metrics 'regex:^smsp__sass_thread_inst_executed_op_(dadd|dmul|dfma)_pred_on.sum$$,gpu__time_duration.sum'\
			--csv --cache-control none --clock-control none --launch-count $(LAUNCH_COUNT) $^ $(LAUNCH_COUNT) | grep -E '^"' | tee $@
