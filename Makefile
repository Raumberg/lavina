SHELL := /bin/bash
BOOTSTRAP_SRC = src/scanner.lv src/parser.lv src/checker.lv src/codegen.lv src/main.lv
STAGES_DIR = stages

# Find the latest stageN.cpp by numeric sort
LATEST_STAGE := $(shell ls $(STAGES_DIR)/stage*.cpp 2>/dev/null | sort -V | tail -1)

# ── Bootstrap from saved stage ───────────────────────────────

bootstrap: $(BOOTSTRAP_SRC)
	@if [ -z "$(LATEST_STAGE)" ]; then echo "No stages found in $(STAGES_DIR)/"; exit 1; fi
	@echo "Bootstrapping from $(LATEST_STAGE)"
	cp runtime/lavina.h /tmp/lavina.h
	cp -r runtime/liblavina /tmp/liblavina
	g++ -std=c++23 -I/tmp -o /tmp/lavina_prev $(LATEST_STAGE)
	/tmp/lavina_prev --emit-cpp src/main.lv > /tmp/lavina_next.cpp
	g++ -std=c++23 -I/tmp -o /tmp/lavina_next /tmp/lavina_next.cpp
	/tmp/lavina_next --emit-cpp src/main.lv > /tmp/lavina_verify.cpp
	@diff -q /tmp/lavina_next.cpp /tmp/lavina_verify.cpp && echo "Fixed point OK" || (echo "MISMATCH" && exit 1)
	@echo "Bootstrap successful."

# ── Save a new stage snapshot ────────────────────────────────

snapshot: bootstrap
	$(eval NEXT_NUM := $(shell echo $(LATEST_STAGE) | grep -o '[0-9]*' | tail -1 | awk '{print $$1+1}'))
	@if diff -q /tmp/lavina_next.cpp $(LATEST_STAGE) > /dev/null 2>&1; then \
		echo "No changes — $(LATEST_STAGE) is already up to date."; \
	else \
		read -p "Stage $(NEXT_NUM) description: " DESC; \
		echo "// Stage $(NEXT_NUM): $$DESC" > $(STAGES_DIR)/stage$(NEXT_NUM).cpp; \
		cat /tmp/lavina_next.cpp >> $(STAGES_DIR)/stage$(NEXT_NUM).cpp; \
		echo "Saved $(STAGES_DIR)/stage$(NEXT_NUM).cpp"; \
	fi

# ── Evolve: bootstrap with codegen changes ──────────────────
# When codegen output changes, the old stage and new compiler
# produce different C++. This target verifies the new output is
# a fixed point of itself and saves it as a new stage.

evolve: $(BOOTSTRAP_SRC)
	@if [ -z "$(LATEST_STAGE)" ]; then echo "No stages found in $(STAGES_DIR)/"; exit 1; fi
	@echo "Bootstrapping from $(LATEST_STAGE)"
	cp runtime/lavina.h /tmp/lavina.h
	cp -r runtime/liblavina /tmp/liblavina
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
			CUR_NUM=$$(echo $(LATEST_STAGE) | grep -o '[0-9]*' | tail -1); \
			NEXT_NUM=$$((CUR_NUM + 1)); \
			read -p "Stage $$NEXT_NUM description: " DESC; \
			echo "// Stage $$NEXT_NUM: $$DESC" > $(STAGES_DIR)/stage$$NEXT_NUM.cpp; \
			cat /tmp/lavina_verify.cpp >> $(STAGES_DIR)/stage$$NEXT_NUM.cpp; \
			cp /tmp/lavina_verify.cpp /tmp/lavina_next.cpp; \
			cp /tmp/lavina_verify /tmp/lavina_next; \
			echo "Saved $(STAGES_DIR)/stage$$NEXT_NUM.cpp"; \
			echo "Evolution successful."; \
		else \
			echo "ERROR: new output is NOT a fixed point. The compiler does not converge."; \
			exit 1; \
		fi; \
	fi

# ── Run test suite ───────────────────────────────────────────

test:
	@if [ ! -f /tmp/lavina_next ]; then echo "Run 'make bootstrap' first"; exit 1; fi
	@cp runtime/lavina.h /tmp/lavina.h
	@rm -rf /tmp/liblavina && cp -r runtime/liblavina /tmp/liblavina
	@passed=0; failed=0; errors=""; \
	for f in tests/test_*.lv; do \
		name=$$(basename $$f .lv); \
		/tmp/lavina_next --emit-cpp $$f > /tmp/$$name.cpp 2>/dev/null && \
		g++ -std=c++23 -I/tmp -o /tmp/$$name /tmp/$$name.cpp 2>/dev/null && \
		/tmp/$$name 2>/dev/null; \
		if [ $$? -eq 0 ]; then \
			echo "  PASS  $$name"; \
			passed=$$((passed + 1)); \
		else \
			echo "  FAIL  $$name"; \
			failed=$$((failed + 1)); \
			errors="$$errors $$name"; \
		fi; \
	done; \
	echo ""; \
	echo "$$passed passed, $$failed failed"; \
	if [ $$failed -ne 0 ]; then echo "Failed:$$errors"; exit 1; fi

clean:
	rm -f /tmp/lavina_prev /tmp/lavina_next /tmp/lavina_next.cpp /tmp/lavina_verify.cpp
	rm -f /tmp/lavina.h
	rm -rf /tmp/liblavina

.PHONY: bootstrap snapshot evolve clean test
