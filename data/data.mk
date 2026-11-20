# data.mk: the program image's .data/.rodata (data/), included by the Makefile. Generated layout: data/d_<RAM>.c
# (one per run of consecutive objects), data/rsp.s (RSP microcode), data/extract.tsv (what `make extract` writes).
# The RSP microcode is assembled from assets/rsp/$(GAME)/*.s (written by `make extract`, g2's RSP disassembly) by
# tools/assets/assets.py rspbin into $(BUILD)/rsp/*.bin; data/rsp.s places those at their addresses.
DATA_EXTRACTED ?= extracted/data
DATA_C      := $(wildcard data/d_*.c)
DATA_OBJS   := $(patsubst data/%.c,$(BUILD)/data/%.o,$(DATA_C)) $(BUILD)/data/rsp.o
RSP_BINS    := $(addprefix $(BUILD)/rsp/,rspboot.bin gspF3DEX.text.bin aspMain.text.bin gspF3DEX.data.bin aspMain.data.bin)
DATA_INCS   := $(patsubst %,$(BUILD)/data/tex/%.inc,$(shell awk '!/^#/ && $$2 == "png" { print $$1 }' data/extract.tsv))
DATA_CFLAGS := -G0 -mips3 -mgp32 -mfp32 -Idata -I$(BUILD)/data

# textures as PNG, from the user's ROM (its SHA-1 must be $(GAME).sha1's)
extract: extract-data
extract-data:
	@test -n "$(BASEROM)" || { echo "usage: make extract BASEROM=path/to/baserom.z64" >&2; exit 1; }
	$(PYTHON) tools/data_tex.py extract "$(BASEROM)" $(GAME).sha1 data/extract.tsv $(DATA_EXTRACTED)
$(DATA_EXTRACTED)/%.png:
	@echo "$@ is missing: run 'make extract BASEROM=<path to the ROM>' first" >&2; exit 1

# texture arrays for #include, generated from the PNGs at build time
$(BUILD)/data/tex/%.inc: $(DATA_EXTRACTED)/%.png tools/data_tex.py data/extract.tsv
	@mkdir -p $(dir $@)
	$(PYTHON) tools/data_tex.py inc $< `awk '$$1 == "$*" { print $$5, $$6, $$7 }' data/extract.tsv` $@

$(BUILD)/data/%.o: data/%.c data/data.h data/gbi.h $(DATA_INCS) tools/datatrim.py data/datatrim.tsv
	@mkdir -p $(dir $@)
	tools/cc.sh kmc+as "$(DATA_CFLAGS)" $< $@
	$(PYTHON) tools/datatrim.py $@ data/datatrim.tsv

# RSP microcode: assembled from the extracted RSP assembly (no ROM access: assets.py's audit hook)
$(RSP_BINS) &: $(ASSETS)/.extracted tools/assets/assets.py tools/assets/owners/g2/tools/rsp_g2.py
	@mkdir -p $(BUILD)/rsp
	$(ASSETPY) rspbin $(GAME) $(ASSETS) $(BUILD)/rsp
$(BUILD)/data/rsp.o: data/rsp.s $(RSP_BINS)
	@mkdir -p $(dir $@)
	$(CROSS)as -EB -mips3 -I$(BUILD)/rsp -o $@ $<

# main segment: code + .data + .rodata (ROM 0x1000 to the end of .rodata)
main: $(BUILD)/$(GAME).main.bin
$(BUILD)/$(GAME).main.bin: $(BUILD)/$(GAME).code.elf
	$(CROSS)objcopy -O binary -j .code -j .main_data -j .main_rodata $< $@

# the data part alone (from the end of the code: microcode, .data, .rodata), for the ROM layout (tools/assets/assets.mk DATA_BIN)
$(BUILD)/$(GAME).data.bin: $(BUILD)/$(GAME).code.elf
	$(CROSS)objcopy -O binary -j .main_data -j .main_rodata $< $@

.PHONY: main extract-data
