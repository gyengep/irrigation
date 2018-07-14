
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
	mkdir -p /usr/share/irrigation
	mkdir -p /var/run/irrigation
	mkdir -p /var/log/irrigation
	cp ./scripts/irrigation_service /etc/init.d/irrigation_service
	chmod 755 /etc/init.d/irrigation_service
	cp ./bin/irrigation /usr/share/irrigation/irrigation
	chmod 555 /usr/share/irrigation/irrigation
	cp ./scripts/functions /usr/share/irrigation/functions
	chmod 644 /usr/share/irrigation/functions
	cp ./data/irrigation.xml /usr/share/irrigation/irrigation.xml
	chmod 644 /usr/share/irrigation/irrigation.xml

.PHONY: uninstall-rpi
uninstall-rpi:
	service irrigation_service stop
	rm -f  /etc/init.d/irrigation_service
	rm -rf /var/run/irrigation/
	rm -rf /var/log/irrigation/
	rm -rf /usr/share/irrigation/
	
.PHONY: install-cygwin
install-cygwin:
	mkdir -p /usr/share/irrigation
	mkdir -p /var/run/irrigation
	mkdir -p /var/log/irrigation
	cp ./scripts/irrigation_service ~/bin/irrigation_service
	chmod 755 ~/bin/irrigation_service
	cp ./bin/irrigation.exe /usr/share/irrigation/irrigation.exe
	chmod 555 /usr/share/irrigation/irrigation.exe
	cp ./scripts/functions /usr/share/irrigation/functions
	chmod 644 /usr/share/irrigation/functions
	cp ./data/irrigation.xml /usr/share/irrigation/irrigation.xml
	chmod 644 /usr/share/irrigation/irrigation.xml
	
.PHONY: uninstall-cygwin
uninstall-cygwin:
	~/bin/irrigation_service stop
	rm -f ~/bin/irrigation_service
	rm -rf /var/run/irrigation/
	rm -rf /var/log/irrigation/
	rm -rf /usr/share/irrigation/
	