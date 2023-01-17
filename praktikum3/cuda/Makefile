CC ?= gcc
SIZES ?= 1024
TEST_SIZE = 1024
PROFILE_DIR = prof
LAUNCH_COUNT ?= 5

cuda-%: wrapper.cpp default.cu ../util.c
	nvcc -DSIZE=$* $^ -allow-unsupported-compiler -o $@

.PHONY: test
test: test.cpp default.cu ../util.c
	nvcc -DSIZE=$(TEST_SIZE) $^ -allow-unsupported-compiler -fmad=false -o $@
	./$@

.PHONY: clean
clean:
	rm -f *.o cuda-* test
	rm -rf $(PROFILE_DIR)

$(PROFILE_DIR):
	mkdir $(PROFILE_DIR)

.PHONY: profile
profile: $(addprefix $(PROFILE_DIR)/profile-,$(SIZES)) | $(PROFILE_DIR)
	../convert.py $^ $(PROFILE_DIR)/profile.csv

$(PROFILE_DIR)/profile-%: cuda-% | $(PROFILE_DIR)
	sudo ncu --metrics 'regex:^smsp__sass_thread_inst_executed_op_(dadd|dmul|dfma)_pred_on.sum$$,gpu__time_duration.sum'\
			--csv --cache-control none --clock-control none --launch-count $(LAUNCH_COUNT) $^ $(LAUNCH_COUNT) | grep -E '^"' | tee $@
