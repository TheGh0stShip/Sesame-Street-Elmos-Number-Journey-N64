# Sesame Street N64 matching decompilation. GAME comes from game.mk.
include game.mk

TOOLROOT ?= $(HOME)/.cache/n64dk-lab/toolroot
KMC      ?= $(TOOLROOT)/kmc-gcc-2.7.2
IDO53    ?= $(TOOLROOT)/ido5.3
IDO71    ?= $(TOOLROOT)/ido7.1
BINUTILS ?= $(TOOLROOT)/mips-binutils/usr
PYTHON   ?= python3
CROSS    := LD_LIBRARY_PATH=$(BINUTILS)/lib/x86_64-linux-gnu $(BINUTILS)/bin/mips-linux-gnu-
export KMC IDO53 IDO71 BINUTILS PYTHON

BUILD ?= build
ROM   := $(BUILD)/$(GAME).z64
HEADERS := $(wildcard include/*.h)

# `make`: code segment (checked), every asset re-encoded from assets/, the full ROM laid out and checked.
# (the .sha1 files name build/...; a BUILD=<dir> build checks its own files)
SHA1CHECK = sed 's\#build/\#$(BUILD)/\#' $(1) | sha1sum -c
all: code main
	$(call SHA1CHECK,$(GAME).code.sha1 $(GAME).main.sha1)
	$(MAKE) rom

rom: $(ROM)
	$(call SHA1CHECK,$(GAME).sha1)

# extract / assets / $(ROM) rules (agent pa): tools/assets/assets.mk
include tools/assets/assets.mk

# per-file lane and flags (tools/flags.tsv) -> make variables
$(BUILD)/flags.mk: tools/flags.tsv
	@mkdir -p $(BUILD)
	awk -F'\t' '!/^#/ { o=$$1; sub(/\.[cs]$$/, ".o", o); print "OBJS += $(BUILD)/" o; \
	  print "$(BUILD)/" o ": LANE := " $$4; print "$(BUILD)/" o ": FLAGS := " $$5; print "$(BUILD)/" o ": SIZE := " $$3 }' $< > $@
-include $(BUILD)/flags.mk
# program .data/.rodata from data/ (rules in data/data.mk)
include data/data.mk

$(BUILD)/src/%.o: src/%.c $(HEADERS) tools/trimtext.py tools/rodata_split.tsv tools/datatrim.py tools/datatrim.tsv | $(BUILD)/flags.mk
	@mkdir -p $(dir $@)
	tools/cc.sh "$(LANE)" "$(FLAGS)" $< $@
	$(PYTHON) tools/trimtext.py $@ $(SIZE)
	$(PYTHON) tools/rodata_split.py $@ tools/rodata_split.tsv
	$(PYTHON) tools/datatrim.py $@ tools/datatrim.tsv

$(BUILD)/asm/%.o: asm/%.s include/asm.h tools/trimtext.py | $(BUILD)/flags.mk
	@mkdir -p $(dir $@)
	tools/as.sh "$(LANE)" $< $@
	$(PYTHON) tools/trimtext.py $@ $(SIZE)

objects: $(OBJS)

# code segment: GNU ld with the generated linker script (every object at its address)
# the linker script names objects under build/; a BUILD=<dir> build links its own objects
$(BUILD)/$(GAME).ld: $(GAME).ld
	@mkdir -p $(BUILD)
	sed 's#build/#$(BUILD)/#g' $< > $@

$(BUILD)/$(GAME).code.elf: $(OBJS) $(DATA_OBJS) $(BUILD)/$(GAME).ld symbols.ld
	$(CROSS)ld -L. -T $(BUILD)/$(GAME).ld --no-check-sections -Map $(BUILD)/$(GAME).map -o $@ $(OBJS) $(DATA_OBJS)

$(BUILD)/$(GAME).code.bin: $(BUILD)/$(GAME).code.elf
	$(CROSS)objcopy -O binary -j .code $< $@

code: $(BUILD)/$(GAME).code.bin

.PHONY: all rom objects code extract clean
clean:
	rm -rf $(BUILD)
