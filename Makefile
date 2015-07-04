.PHONY: docs format-examples cloc

docs:
	doxygen doxygen.config

format-examples:
	find examples tests -name '*.ino' -exec astyle --options=/data/formatter.conf {} \;

cloc:
	cloc

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
