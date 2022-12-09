# files are named 
# csv: version-size.csv
# bin: version-size
#

versions = default loopswap unrolling
sizes = 128 256 512 1024 2048
outputdir = timing
bindir = bin
allcsv = $(addsuffix .csv,$(foreach S,$(versions),$(addprefix $(outputdir)/$S-,$(sizes))))

.PHONY: all clean #$(allcsv)
all: $(outputdir)/data.csv

$(outputdir)/data.csv: $(allcsv) | $(outputdir)
	python3 convert.py $^ $(outputdir)/data.csv

$(outputdir)/%.csv: $(bindir)/% | $(outputdir)
	if [ ! -f $@ ]; then echo "values" > $@; fi	
	while [ $$(wc -l < $@) -lt 11 ]; do $< >> $@; done

$(bindir):
	mkdir $(bindir)
$(outputdir):
	mkdir $(outputdir)

clean:
	rm -r $(outputdir) $(bindir)
# executable
.SECONDEXPANSION:
$(bindir)/%: wrapper.c $$(addsuffix .c,$$(word 1, $$(subst -, ,%))) | $(bindir) #$$(*F)
	gcc -DSIZE="$(word 2, $(subst -, ,$(*F)))" $^ -o $@

# $(outputdir)%.csv: $$(word 1,$$(subst -, ,$$(*F)))
# 	echo "name is $(*F) preq $^"
