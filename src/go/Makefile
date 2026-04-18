# Build two variants of the G6502PP (cpp -> Go) interpreter:
#   bin/G65O2PP_nopin  (X/Y/S accessed through cpu struct, no register pinning)
#   bin/G65O2PP_pin    (X/Y/S pinned to locals via -DPIN_ALL)
#
# Each variant is preprocessed separately (so cpp sees the right macro set)
# then compiled with the same aggressive Go flags.
#
# Every build gates on `go vet` and (if installed) staticcheck/golangci-lint
# against the preprocessed build/<variant>/processed.go; any finding fails
# `make all`. Stamps under build/<variant>/ mean checks only rerun when the
# preprocessed output actually changes.
#
# Fixes land in the .gh / main.go sources — build/*/processed.go is generated.

CC      := gcc
GO      := go

# -s/-w: strip symbol + DWARF info
# -B: disable bounds checks, -l: disable inlining budget limits (encourage inline)
GO_LDFLAGS := -ldflags=-s -w
GO_GCFLAGS := -gcflags=all=-B -l

ENTRY    := src/main.go
GH_DEPS  := $(wildcard src/*.gh)

VARIANTS  := nopin pin
BINS      := $(VARIANTS:%=bin/G65O2PP_%)
PROCESSED := $(VARIANTS:%=build/%/processed.go)
VET_STAMP := $(VARIANTS:%=build/%/.vet.ok)
LINT_STAMP:= $(VARIANTS:%=build/%/.lint.ok)

# Per-variant cpp flag. Extend here for new variants.
CPPFLAGS_pin   := -DPIN_ALL
CPPFLAGS_nopin :=

# goimports consolidates the split import blocks cpp emits; fall back to gofmt.
FMT := $(shell command -v goimports 2>/dev/null || echo gofmt)

# Prefer staticcheck; accept golangci-lint; degrade (with warning) if neither.
LINTER := $(shell command -v staticcheck 2>/dev/null || command -v golangci-lint 2>/dev/null)

.PHONY: all clean $(VARIANTS) fmt vet fix lint check
.SECONDARY: $(PROCESSED) $(VET_STAMP) $(LINT_STAMP)

all: $(BINS)

$(VARIANTS): %: bin/G65O2PP_%

bin/G65O2PP_%: build/%/processed.go build/%/.vet.ok build/%/.lint.ok
	@mkdir -p $(@D)
	$(GO) build "$(GO_LDFLAGS)" "$(GO_GCFLAGS)" -trimpath -o $@ $<

# -P: no linemarkers, -xc: C mode, -undef: no built-in system macros.
build/%/processed.go: $(ENTRY) $(GH_DEPS)
	@mkdir -p $(@D)
	$(CC) -E -P -xc -undef $(CPPFLAGS_$*) $(ENTRY) -o $@
	$(FMT) -w $@

build/%/.vet.ok: build/%/processed.go
	@echo "  [vet] $<"
	@$(GO) vet $<
	@touch $@

build/%/.lint.ok: build/%/processed.go
	@if [ -n "$(LINTER)" ]; then \
	    echo "  [lint] $< ($(notdir $(LINTER)))"; \
	    case "$(notdir $(LINTER))" in \
	        staticcheck)     $(LINTER) $< ;; \
	        golangci-lint)   $(LINTER) run $< ;; \
	    esac; \
	else \
	    echo "  [lint] $< skipped (no staticcheck or golangci-lint in PATH)"; \
	fi
	@touch $@

# --- Phony convenience targets ----------------------------------------------

fmt: $(PROCESSED)
	@for f in $^; do echo "  [fmt] $$f"; $(FMT) -w $$f; done

vet: $(VET_STAMP)

lint: $(LINT_STAMP)

# `go fix` is advisory only: it prints suggestions that must be hand-applied
# back into the .gh / main.go sources, so it is never gated on the build.
fix: $(PROCESSED)
	@for f in $^; do echo "  [fix] $$f (diff only — apply to .gh/main.go)"; $(GO) fix -diff $$f; done

check: fmt vet lint fix

clean:
	rm -rf build bin
