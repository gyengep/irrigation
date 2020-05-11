
PROJECT_ROOT    = ..

.PHONY: all
all:
	@$(MAKE) -C DocumentView all
	@$(MAKE) -C DtoReaderWriter all
	@$(MAKE) -C Hardware/DS18B20 all
	@$(MAKE) -C Hardware/Valves all
	@$(MAKE) -C Logger all
	@$(MAKE) -C Logic all
	@$(MAKE) -C Model all
	@$(MAKE) -C Schedulers all
	@$(MAKE) -C Utils all
	@$(MAKE) -C Views/RestView all
	@$(MAKE) -C Views/TimerView all
	@$(MAKE) -C WebServer all

.PHONY: clean
clean:
	@$(MAKE) -C DocumentView clean
	@$(MAKE) -C DtoReaderWriter clean
	@$(MAKE) -C Hardware/DS18B20 clean
	@$(MAKE) -C Hardware/Valves clean
	@$(MAKE) -C Logger clean
	@$(MAKE) -C Logic clean
	@$(MAKE) -C Model clean
	@$(MAKE) -C Schedulers clean
	@$(MAKE) -C Utils clean
	@$(MAKE) -C Views/RestView clean
	@$(MAKE) -C Views/TimerView clean
	@$(MAKE) -C WebServer clean

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
