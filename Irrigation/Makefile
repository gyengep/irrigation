
.PHONY: all
all: library-all test-all src-all

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

.PHONY: install
install:
	@./scripts/install.sh
	
.PHONY: uninstall
uninstall:
	@./scripts/uninstall.sh
