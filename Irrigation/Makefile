
.PHONY: all
all: library-all test-all src-all

.PHONY: all-without-test
all-without-test: library-all src-all

.PHONY: clean
clean: library-clean test-clean src-clean

.PHONY: library-all
library-all:
	@$(MAKE) -C lib all
	
.PHONY: library-clean
library-clean:
	@$(MAKE) -C lib clean

.PHONY: test-all
test-all:
	@$(MAKE) -C test all
	
.PHONY: test-clean
test-clean:
	@$(MAKE) -C test clean

.PHONY: src-all
src-all:
	@$(MAKE) -C src all
	
.PHONY: src-clean
src-clean:
	@$(MAKE) -C src clean

.PHONY: gmock
gmock:
	@$(MAKE) -C 3p/gmock_gtest all
	
.PHONY: pugixml
pugixml:
	@$(MAKE) -C 3p/pugixml all

.PHONY: install-rpi
install-rpi:
	mkdir /usr/share/irrigation
	chown pi:pi /usr/share/irrigation
	cp .bin/irrigation /usr/share/irrigation/
	cp .scripts/functions /usr/share/irrigation/
	cp .data/irrigation.xml /usr/share/irrigation/
	
.PHONY: install-cygwin
install-cygwin:
	mkdir -p /usr/share/irrigation
	mkdir -p /var/run/irrigation
	cp ./scripts/irrigation_service-cygwin ~/bin/irrigation_service
	cp ./bin/irrigation.exe /usr/share/irrigation/
	cp ./scripts/functions /usr/share/irrigation/
	cp ./data/irrigation.xml /usr/share/irrigation/	