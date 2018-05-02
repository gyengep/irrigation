
all: 	irrigation test

clean: 	irrigation-clean test-clean

.PHONY:	irrigation test

irrigation:
	@$(MAKE) -C src all

irrigation-clean:
	@$(MAKE) -C src clean

test:
	@$(MAKE) -C test all
	
test-clean:
	@$(MAKE) -C test clean
