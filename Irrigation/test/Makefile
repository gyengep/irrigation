
PROJECT_ROOT    = ..


.PHONY: all
all: library test

.PHONY: clean
clean: library-clean test-clean
	
.PHONY: library
library:
	@$(MAKE) -C $(PROJECT_ROOT)/src library
	
.PHONY: library-clean
library-clean:
	@$(MAKE) -C $(PROJECT_ROOT)/src library-clean
	
.PHONY: test
test: 
	@$(MAKE) -C $(PROJECT_ROOT)/test/CommandExecutor test
	@$(MAKE) -C $(PROJECT_ROOT)/test/DocumentView test
	@$(MAKE) -C $(PROJECT_ROOT)/test/Hardware test
	@$(MAKE) -C $(PROJECT_ROOT)/test/Logger test
	@$(MAKE) -C $(PROJECT_ROOT)/test/Logic test
	@$(MAKE) -C $(PROJECT_ROOT)/test/Schedulers test
	#@$(MAKE) -C $(PROJECT_ROOT)/test/Utils test
	#@$(MAKE) -C $(PROJECT_ROOT)/test/Views/CommandLineView test

.PHONY: test-clean
test-clean:
	@$(MAKE) -C $(PROJECT_ROOT)/test/CommandExecutor test-clean
	@$(MAKE) -C $(PROJECT_ROOT)/test/DocumentView test-clean
	@$(MAKE) -C $(PROJECT_ROOT)/test/Hardware test-clean
	@$(MAKE) -C $(PROJECT_ROOT)/test/Logger test-clean
	@$(MAKE) -C $(PROJECT_ROOT)/test/Logic test-clean
	@$(MAKE) -C $(PROJECT_ROOT)/test/Schedulers test-clean
	#@$(MAKE) -C $(PROJECT_ROOT)/test/Utils test-clean
	#@$(MAKE) -C $(PROJECT_ROOT)/test/Views/CommandLineView test-clean

