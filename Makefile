.PHONY: docs format-examples cloc travis-build examples update-travis

docs:
	doxygen extras/doxygen.config

format-examples:
	find examples tests -name '*.ino' -exec astyle --options=extras/formatter.conf {} \;

cloc:
	cloc ./

examples:
	python extras/build-examples.py

update-travis:
	python extras/travis-generate.py > .travis.yml.new
	meld .travis.yml .travis.yml.new

travis-build:
ifdef PLATFORMIO_CI_SRC
	-rm -rf ./linux
ifdef PLATFORMIO_CI_BOARDS_ARGS
	platformio ci --lib="." $(PLATFORMIO_CI_BOARDS_ARGS)
else
	platformio ci --lib="." --board=uno --board=leonardo
endif
endif
ifdef LINUX_SRC
	make -C $(LINUX_SRC)
endif
