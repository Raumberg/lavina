SHELL := /bin/bash
BOOTSTRAP_SRC = src/scanner.lv src/parser.lv src/checker.lv src/codegen.lv src/main.lv
LATEST_STAGE = stages/stage-latest.cpp

# ── Bootstrap from saved stage ───────────────────────────────

bootstrap: $(BOOTSTRAP_SRC)
	@echo "Bootstrapping from $(LATEST_STAGE)"
	cp runtime/lavina.h /tmp/lavina.h
	rm -rf /tmp/liblavina && cp -r runtime/liblavina /tmp/liblavina
	g++ -std=c++23 -I/tmp -o /tmp/lavina_prev $(LATEST_STAGE)
	/tmp/lavina_prev --emit-cpp src/main.lv > /tmp/lavina_next.cpp
	g++ -std=c++23 -I/tmp -o /tmp/lavina_next /tmp/lavina_next.cpp
	/tmp/lavina_next --emit-cpp src/main.lv > /tmp/lavina_verify.cpp
	@diff -q /tmp/lavina_next.cpp /tmp/lavina_verify.cpp && echo "Fixed point OK" || (echo "MISMATCH" && exit 1)
	@echo "Bootstrap successful."

# ── Save a new stage snapshot ────────────────────────────────

snapshot: bootstrap
	@if diff -q /tmp/lavina_next.cpp $(LATEST_STAGE) > /dev/null 2>&1; then \
		echo "No changes — stage-latest.cpp is already up to date."; \
	else \
		cp /tmp/lavina_next.cpp $(LATEST_STAGE); \
		echo "Updated $(LATEST_STAGE)"; \
	fi

# ── Evolve: bootstrap with codegen changes ──────────────────

evolve: $(BOOTSTRAP_SRC)
	@echo "Bootstrapping from $(LATEST_STAGE)"
	cp runtime/lavina.h /tmp/lavina.h
	rm -rf /tmp/liblavina && cp -r runtime/liblavina /tmp/liblavina
	g++ -std=c++23 -I/tmp -o /tmp/lavina_prev $(LATEST_STAGE)
	/tmp/lavina_prev --emit-cpp src/main.lv > /tmp/lavina_next.cpp
	g++ -std=c++23 -I/tmp -o /tmp/lavina_next /tmp/lavina_next.cpp
	/tmp/lavina_next --emit-cpp src/main.lv > /tmp/lavina_verify.cpp
	@if diff -q /tmp/lavina_next.cpp /tmp/lavina_verify.cpp > /dev/null 2>&1; then \
		echo "Fixed point OK — no evolution needed (use 'make bootstrap')."; \
	else \
		echo "Codegen changed — verifying new output is a fixed point..."; \
		g++ -std=c++23 -I/tmp -o /tmp/lavina_verify /tmp/lavina_verify.cpp; \
		/tmp/lavina_verify --emit-cpp src/main.lv > /tmp/lavina_verify2.cpp; \
		if diff -q /tmp/lavina_verify.cpp /tmp/lavina_verify2.cpp > /dev/null 2>&1; then \
			echo "New fixed point OK."; \
			cp /tmp/lavina_verify.cpp $(LATEST_STAGE); \
			cp /tmp/lavina_verify.cpp /tmp/lavina_next.cpp; \
			cp /tmp/lavina_verify /tmp/lavina_next; \
			echo "Updated $(LATEST_STAGE)"; \
			echo "Evolution successful."; \
		else \
			echo "ERROR: new output is NOT a fixed point. The compiler does not converge."; \
			exit 1; \
		fi; \
	fi

# ── Run test suite ───────────────────────────────────────────

SKIP_WINDOWS_TESTS = test_std_fs test_std_os test_stdlib

test:
	@if [ ! -f /tmp/lavina_next ]; then echo "Run 'make bootstrap' first"; exit 1; fi
	@cp runtime/lavina.h /tmp/lavina.h
	@rm -rf /tmp/liblavina && cp -r runtime/liblavina /tmp/liblavina
	@passed=0; failed=0; skipped=0; errors=""; \
	for f in tests/test_*.lv; do \
		name=$$(basename $$f .lv); \
		dir=$$(dirname $$f); \
		if [ -n "$$MSYSTEM" ]; then \
			skip=0; \
			for s in $(SKIP_WINDOWS_TESTS); do \
				if [ "$$name" = "$$s" ]; then skip=1; break; fi; \
			done; \
			if [ $$skip -eq 1 ]; then \
				echo "  SKIP  $$name  (Windows)"; \
				skipped=$$((skipped + 1)); \
				continue; \
			fi; \
		fi; \
		/tmp/lavina_next compile $$f 2>/dev/null && \
		$$dir/$$name 2>/dev/null; \
		if [ $$? -eq 0 ]; then \
			echo "  PASS  $$name"; \
			passed=$$((passed + 1)); \
		else \
			echo "  FAIL  $$name"; \
			failed=$$((failed + 1)); \
			errors="$$errors $$name"; \
		fi; \
		rm -f $$dir/$$name $$dir/$$name.cpp; \
	done; \
	echo ""; \
	if [ $$skipped -gt 0 ]; then \
		echo "$$passed passed, $$failed failed, $$skipped skipped"; \
	else \
		echo "$$passed passed, $$failed failed"; \
	fi; \
	if [ $$failed -ne 0 ]; then echo "Failed:$$errors"; exit 1; fi

# ── Build compiler binary from latest stage ─────────────────

build:
	@echo "Building from $(LATEST_STAGE)"
	@mkdir -p build
	cp runtime/lavina.h /tmp/lavina.h
	rm -rf /tmp/liblavina && cp -r runtime/liblavina /tmp/liblavina
	g++ -std=c++23 -O2 -I/tmp -o build/lavina $(LATEST_STAGE)
	@echo "Built build/lavina"

# ── Build lvpkg package manager ──────────────────────────────

lvpkg:
	@if [ ! -f /tmp/lavina_next ]; then echo "Run 'make bootstrap' first"; exit 1; fi
	@mkdir -p build
	cp runtime/lavina.h /tmp/lavina.h
	rm -rf /tmp/liblavina && cp -r runtime/liblavina /tmp/liblavina
	/tmp/lavina_next --emit-cpp lvpkg/lvpkg.lv > /tmp/lvpkg.cpp
	g++ -std=c++23 -O2 -I/tmp -o build/lvpkg /tmp/lvpkg.cpp
	@echo "Built build/lvpkg"

# ── Install ──────────────────────────────────────────────────

PREFIX ?= /usr/local

install: build lvpkg
	mkdir -p $(PREFIX)/bin
	cp build/lavina $(PREFIX)/bin/lavina
	cp build/lvpkg $(PREFIX)/bin/lvpkg
	mkdir -p $(PREFIX)/lib/lavina/runtime/liblavina
	mkdir -p $(PREFIX)/lib/lavina/runtime/std
	cp runtime/lavina.h $(PREFIX)/lib/lavina/runtime/
	cp runtime/liblavina/*.h $(PREFIX)/lib/lavina/runtime/liblavina/
	cp runtime/std/*.lv $(PREFIX)/lib/lavina/runtime/std/
	@echo "Installed lavina to $(PREFIX)/"

uninstall:
	rm -f $(PREFIX)/bin/lavina $(PREFIX)/bin/lvpkg
	rm -rf $(PREFIX)/lib/lavina
	@echo "Uninstalled lavina from $(PREFIX)/"

clean:
	rm -f /tmp/lavina_prev /tmp/lavina_next /tmp/lavina_next.cpp /tmp/lavina_verify.cpp
	rm -f /tmp/lavina.h
	rm -rf /tmp/liblavina

.PHONY: bootstrap snapshot evolve clean test build lvpkg install uninstall
