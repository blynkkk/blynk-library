.PHONY: docs format-examples cloc travis-build examples

docs:
	doxygen doxygen.config

format-examples:
	find examples tests -name '*.ino' -exec astyle --options=formatter.conf {} \;

cloc:
	cloc ./

examples:
	python tests/build-examples.py

travis-build:
ifdef PLATFORMIO_CI_SRC
	-rm -rf ./linux
ifdef PLATFORMIO_CI_BOARDS_ARGS
	platformio ci --lib="." $(PLATFORMIO_CI_BOARDS_ARGS)
else
	platformio ci --lib="." --board=uno --board=leonardo --board=yun --board=megaatmega2560
endif
endif
ifdef LINUX_SRC
	make -C $(LINUX_SRC)
endif
