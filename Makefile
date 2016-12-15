.PHONY: docs format-examples cloc travis-build examples update-travis update-ver spaces clean check

docs:
	doxygen extras/doxygen.config

format-examples:
	find examples tests -name '*.ino' -exec astyle --options=extras/formatter.conf {} \;

cloc:
	cloc ./

examples:
	python extras/build-examples.py

spaces:
	find ./src ./linux -name '*.cpp' -o -name '*.h' -exec sed -i 's/\t/    /g' {} \;
	find ./examples ./tests -name '*.ino' -o -name '*.h' -exec sed -i 's/\t/  /g' {} \;

update-travis:
	python extras/travis-generate.py > .travis.yml.new
	meld .travis.yml .travis.yml.new

update-ver:
	sed -i 's/"version": "[0-9\.]*"/"version": "$(VER)"/g' library.json
	sed -i 's/version=[0-9\.]*/version=$(VER)/g' library.properties
	sed -i 's/BLYNK_VERSION        "[0-9\.]*"/BLYNK_VERSION        "$(VER)"/g' src/Blynk/BlynkConfig.h

clean:
	-rm .travis.yml.new
	-find . -type f -name "*.orig" -exec rm -f {} \;

check: spaces format-examples
	@echo '******************************************'
	@echo ' Files with no "YourAuthToken":'
	@echo '******************************************'
	@find ./examples -name "*.ino" -exec grep -L "YourAuthToken" {} \;

travis-build:
ifdef PLATFORMIO_CI_SRC
	-rm -rf ./linux
ifdef PLATFORMIO_CI_EXTRA_ARGS
	platformio ci --lib="." $(PLATFORMIO_CI_EXTRA_ARGS)
else
	platformio ci --lib="." --board=uno
endif
endif
ifdef LINUX_SRC
	make -C $(LINUX_SRC)
endif
