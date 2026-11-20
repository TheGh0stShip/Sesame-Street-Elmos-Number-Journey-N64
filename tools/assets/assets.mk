# Assets and the final ROM (included by the Makefile). Only `make extract` reads the base ROM.
#   make extract BASEROM=baserom.z64   ROM -> assets/<owner>/ (decoded files), once
#   make assets                        assets/ -> build/assets/<owner>.pcs (every owner re-encoded, no ROM access)
#   make rom                           + main segment + gzip 1.2.4 -> build/$(GAME).z64, sha1sum -c $(GAME).sha1
ASSETS    ?= assets
PCS       := $(BUILD)/assets
ASSETPY   := $(PYTHON) tools/assets/assets.py
ASSET_OWNERS := $(shell $(PYTHON) tools/assets/assets.py owners $(GAME))
ASSET_TOOLS  := $(wildcard tools/assets/*.py) tools/assets/tables/layout_$(GAME).tsv
# the linked main segment: code + .data + .rodata (src/ + data/, rule `main` in data/data.mk)
ROM_MAIN  ?= $(BUILD)/$(GAME).main.bin
# GNU gzip 1.2.4 small-memory build (the packer of the level files); public source, not shipped
GZIP124_TAR ?= $(BUILD)/tools/gzip-1.2.4.tar.gz
GZIP124_URL ?= https://ftp.gnu.org/gnu/gzip/gzip-1.2.4.tar.gz
GZIP124     := $(BUILD)/tools/gzip124_SMALL_MEM

extract:
	@test -n "$(BASEROM)" || { echo "usage: make extract BASEROM=path/to/baserom.z64" >&2; exit 1; }
	$(ASSETPY) extract "$(BASEROM)" $(GAME) $(ASSETS)
	touch $(ASSETS)/.extracted

$(ASSETS)/.extracted:
	@echo "assets/ is empty: run 'make extract BASEROM=path/to/baserom.z64' first" >&2; exit 1

$(PCS)/%.pcs: $(ASSETS)/.extracted $(ASSET_TOOLS)
	@mkdir -p $(PCS)
	$(ASSETPY) encode $(GAME) $(ASSETS) $(PCS) $*

assets: $(patsubst %,$(PCS)/%.pcs,$(ASSET_OWNERS))

$(BUILD)/tools/gzip-1.2.4.tar.gz:
	@mkdir -p $(dir $@)
	curl -fsSL -o $@ $(GZIP124_URL)

$(GZIP124): $(GZIP124_TAR) tools/assets/gzip124.sha1
	@mkdir -p $(BUILD)/tools/src
	cd $(dir $(GZIP124_TAR)) && sha1sum -c $(CURDIR)/tools/assets/gzip124.sha1 --status || { echo "gzip-1.2.4.tar.gz: unexpected SHA-1" >&2; exit 1; }
	tar -xzf $(GZIP124_TAR) -C $(BUILD)/tools/src
	cd $(BUILD)/tools/src/gzip-1.2.4 && $(CC) -std=gnu89 -w -O1 -DNO_ASM -DSTDC_HEADERS=1 -DHAVE_UNISTD_H=1 -DDIRENT=1 \
	  -DNO_UTIME -DSMALL_MEM -o $(CURDIR)/$@ gzip.c zip.c deflate.c trees.c bits.c unzip.c inflate.c util.c crypt.c \
	  lzw.c unlzw.c unpack.c unlzh.c getopt.c

$(ROM): $(ROM_MAIN) assets $(GZIP124) $(ASSET_TOOLS)
	$(ASSETPY) rom $(GAME) $(ASSETS) $(PCS) $(ROM_MAIN) $(GZIP124) $@

.PHONY: extract assets
