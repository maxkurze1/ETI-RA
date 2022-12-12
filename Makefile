# files are named 
# csv: version-size.csv
# bin: version-size
#

versions = default loopswap unrolling tiling
sizes = 128 256 512 1024 2048
timing = timing
latex = latex
bindir = bin

allcsv = $(addsuffix .csv,$(foreach S,$(versions),$(addprefix $(timing)/$S-,$(sizes))))
allbin = $(foreach S,$(versions),$(addprefix $(bindir)/$S-,$(sizes)))

.PHONY: all clean protokoll
all: $(allcsv)

protokoll: $(latex)/protokoll.pdf

compile: $(allbin)

$(timing)/data.csv: $(allcsv) | $(timing)
	python3 convert.py $^ $(timing)/data.csv

$(timing)/%.csv: $(bindir)/% | $(timing)
	# should be used without 'if' to regenerate if dependency changed -> for now at least touch
	if [ ! -f $@ ]; then echo "values" > $@; fi	
	while [ $$(wc -l < $@) -lt 11 ]; do $< >> $@; done
	@touch $@

$(latex)/protokoll.pdf: protokoll.tex $(timing)/data.csv $(addsuffix .c,$(versions)) | $(latex)
	latexmk -pdflua -outdir=$(latex) -shell-escape protokoll.tex
	@touch $@

$(bindir):
	mkdir $(bindir)
$(timing):
	mkdir $(timing)
$(latex):
	mkdir $(latex)

clean:
	rm -r $(timing) $(bindir) $(latex)

# executable
.SECONDEXPANSION:
$(bindir)/%: wrapper.c util.c $$(addsuffix .c,$$(word 1, $$(subst -, ,%))) | $(bindir) #$$(*F)
	gcc -DSIZE="$(word 2, $(subst -, ,$(*F)))" $^ -o $@

.PHONY: test
test: $(addsuffix -512,$(addprefix $(bindir)/test-,$(versions)))
	@for i in $^; do echo "Testing $$i"; $$i; done

$(bindir)/test-%: testing.c util.c $$(addsuffix .c,$$(word 1, $$(subst -, ,%))) | $(bindir)
	gcc -DSIZE="$(word 2, $(subst -, ,$(*F)))" $^ -lcblas -o $@
