SHELL := /bin/bash
BOOTSTRAP_SRC = src/scanner.lv src/parser.lv src/codegen.lv src/main.lv
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

clean:
	rm -f /tmp/lavina_prev /tmp/lavina_next /tmp/lavina_next.cpp /tmp/lavina_verify.cpp
	rm -f /tmp/lavina.h
	rm -rf /tmp/liblavina

.PHONY: bootstrap snapshot clean
