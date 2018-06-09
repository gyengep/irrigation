
PROJECT_ROOT    = ..

.PHONY: all
all:
	@$(MAKE) -C CommandExecutor all
	@$(MAKE) -C DocumentView all
	@$(MAKE) -C DTO all
	@$(MAKE) -C Hardware all
	@$(MAKE) -C Logger all
	@$(MAKE) -C Logic all
	#@$(MAKE) -C Model all
	@$(MAKE) -C ReaderWriter all
	@$(MAKE) -C Schedulers all
	#@$(MAKE) -C Utils all
	#@$(MAKE) -C Views/CommandLineView all

.PHONY: clean
clean:
	@$(MAKE) -C CommandExecutor clean
	@$(MAKE) -C DocumentView clean
	@$(MAKE) -C DTO clean
	@$(MAKE) -C Hardware clean
	@$(MAKE) -C Logger clean
	@$(MAKE) -C Logic clean
	#@$(MAKE) -C Model clean
	@$(MAKE) -C ReaderWriter clean
	@$(MAKE) -C Schedulers clean
	#@$(MAKE) -C Utils clean
	#@$(MAKE) -C Views/CommandLineView clean

.PHONY: all-with-libraries
all-with-libraries: library-all all

.PHONY: clean-with-libraries
clean-with-libraries: library-clean clean
		
.PHONY: library-all
library-all:
	@$(MAKE) -C $(PROJECT_ROOT)/lib all

.PHONY: library-clean
library-clean:
	@$(MAKE) -C $(PROJECT_ROOT)/lib clean
