build:
	cargo build

test:
	cargo test -- --test-threads=1

probe:
	for f in probes/cpp/*.lv; do echo -n "$f: "; cargo run -- "$f" 2>&1 | grep -c "error" || true; done

bootstrap:
	cat bootstrap/scanner.lv bootstrap/parser.lv > /tmp/combined.lv && cargo run -- /tmp/combined.lv
