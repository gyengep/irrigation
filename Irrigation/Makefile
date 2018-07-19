
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
	#################################
	# DIR /usr/share/irrigation
	#################################
	mkdir -p /usr/share/irrigation
	chown pi:pi /usr/share/irrigation
	# /usr/share/irrigation/irrigation
	cp ./bin/irrigation /usr/share/irrigation/irrigation
	chmod 544 /usr/share/irrigation/irrigation
	chown pi:pi /usr/share/irrigation/irrigation
	# /usr/share/irrigation/irrigation.xml
	cp --no-clobber ./data/irrigation.xml /usr/share/irrigation/irrigation.xml
	chmod 644 /usr/share/irrigation/irrigation.xml
	chown pi:pi /usr/share/irrigation/irrigation.xml
	#################################
	# DIR /var/log/irrigation
	#################################
	mkdir -p /var/log/irrigation
	chown pi:pi /var/log/irrigation
	touch /var/log/irrigation/irrigation.log
	chown pi:pi /var/log/irrigation/irrigation.log
	#################################
	# /etc/init.d/irrigation_service
	#################################
	cp ./scripts/irrigation_service /etc/init.d/irrigation_service
	chmod 755 /etc/init.d/irrigation_service
	update-rc.d irrigation_service defaults

.PHONY: uninstall-rpi
uninstall-rpi:
	service irrigation_service stop
	update-rc.d irrigation_service remove
	rm -f /etc/init.d/irrigation_service
	rm -f /usr/share/irrigation/irrigation
	
.PHONY: install-cygwin
install-cygwin:
	mkdir -p /usr/share/irrigation
	cp ./data/irrigation.xml /usr/share/irrigation/irrigation.xml
	chmod 644 /usr/share/irrigation/irrigation.xml
	mkdir -p /var/log/irrigation
	touch /var/log/irrigation/irrigation.log
	
.PHONY: uninstall-cygwin
uninstall-cygwin:
	rm -rf /usr/share/irrigation/
	rm -rf /var/log/irrigation/
