SHELL := /bin/bash
BOOTSTRAP_SRC = bootstrap/scanner.lv bootstrap/parser.lv bootstrap/codegen.lv bootstrap/main.lv
COMBINED = /tmp/lavina_combined.lv

# Concatenate bootstrap sources, stripping import lines (Rust compiler can't handle them)
define combine
	cat bootstrap/scanner.lv \
		<(grep -v '^import ' bootstrap/parser.lv) \
		<(grep -v '^import ' bootstrap/codegen.lv) \
		<(grep -v '^import ' bootstrap/main.lv) > $(COMBINED)
endef

# ── Rust compiler ────────────────────────────────────────────

build:
	cargo build

test:
	cargo test -- --test-threads=1

probe:
	@for f in probes/cpp/*.lv; do echo -n "$$f: "; cargo run -- "$$f" 2>&1 | grep -c "error" || true; done

# ── Bootstrap: stage 0 (Rust compiler → bootstrap binary) ────

stage0: $(BOOTSTRAP_SRC)
	$(combine)
	cargo run -- --emit-cpp $(COMBINED) 2>/dev/null > /tmp/lavina_stage0.cpp
	cp runtime/lavina.h /tmp/lavina.h
	g++ -std=c++23 -I/tmp -o /tmp/lavina_stage0 /tmp/lavina_stage0.cpp
	@echo "Built /tmp/lavina_stage0"

# ── Bootstrap: stage 1 (stage0 compiles itself via imports) ──

stage1: stage0
	/tmp/lavina_stage0 --emit-cpp bootstrap/main.lv > /tmp/lavina_stage1.cpp
	g++ -std=c++23 -I/tmp -o /tmp/lavina_stage1 /tmp/lavina_stage1.cpp
	@echo "Built /tmp/lavina_stage1"

# ── Bootstrap: stage 2 (stage1 compiles itself, must match) ──

stage2: stage1
	/tmp/lavina_stage1 --emit-cpp bootstrap/main.lv > /tmp/lavina_stage2.cpp
	@diff -q /tmp/lavina_stage1.cpp /tmp/lavina_stage2.cpp && echo "Fixed point OK" || (echo "MISMATCH: stage1 != stage2" && exit 1)

# ── Full bootstrap verification ──────────────────────────────

bootstrap: stage2
	@echo "Bootstrap successful — all stages verified."

# ── Convenience: build the bootstrap compiler ────────────────

bootstrap-bin: stage1
	cp /tmp/lavina_stage1 ./bootstrap/lavina
	@echo "Installed bootstrap/lavina"

clean:
	cargo clean
	rm -f /tmp/lavina_stage*.cpp /tmp/lavina_stage* /tmp/lavina_combined.lv /tmp/lavina.h
	rm -f bootstrap/lavina

.PHONY: build test probe stage0 stage1 stage2 bootstrap bootstrap-bin clean
