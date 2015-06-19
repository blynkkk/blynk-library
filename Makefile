.PHONY: docs format-examples cloc

docs:
	doxygen doxygen.config

format-examples:
	find examples tests -name '*.ino' -exec astyle --options=/data/formatter.conf {} \;

cloc:
	cloc
