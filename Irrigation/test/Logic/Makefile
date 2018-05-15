
TARGET_NAME     = $(shell basename $(CURDIR))

########################################################
########################################################

PROJECT_ROOT    = ../..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/test/$(TARGET_NAME)
OBJECT_DIR      = $(PROJECT_ROOT)/build/test/$(TARGET_NAME)
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/build
LIBRARY_DIR     = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/$(TARGET_NAME)_test.exe
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
DEPENDS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.d))

########################################################
########################################################

LOCAL_LIBS      = -l$(TARGET_NAME) -lHardware -lSchedulers -lLogger
LOCAL_LIB_FILES = $(patsubst -l%,$(LIBRARY_DIR)/lib%.a,$(LOCAL_LIBS))
INCLUDE_PATH    = -I$(INCLUDE_DIR)
LIBDIRS         = -L$(LIBRARY_DIR)
LIBS            = -lgmock $(LOCAL_LIBS)

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0 -MMD -MP 
CPPFLAGS        = -isystem $(PROJECT_ROOT)/lib/gmock_gtest $(INCLUDE_PATH) 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3


.PHONY: all
all: library test

.PHONY: clean
clean: library-clean test-clean
	
.PHONY: library
library:
	@$(MAKE) -C $(PROJECT_ROOT)/src/$(TARGET_NAME) library
	@$(MAKE) -C $(PROJECT_ROOT)/src/Hardware library
	@$(MAKE) -C $(PROJECT_ROOT)/src/Schedulers library
	@$(MAKE) -C $(PROJECT_ROOT)/src/Logger library
	
.PHONY: library-clean
library-clean:
	@$(MAKE) -C $(PROJECT_ROOT)/src/$(TARGET_NAME) library-clean
	@$(MAKE) -C $(PROJECT_ROOT)/src/Hardware library-clean
	@$(MAKE) -C $(PROJECT_ROOT)/src/Schedulers library-clean
	@$(MAKE) -C $(PROJECT_ROOT)/src/Logger library-clean
	
.PHONY: test
test: $(TARGET)
	@echo 'Executing test: $(TARGET)'
	@$(TARGET)
	@echo 'Finished executing test: $(TARGET)'
	@echo ' ' 

.PHONY: test-clean
test-clean:
	rm -f $(OBJECTS)
	rm -f $(DEPENDS)
	rm -f $(TARGET)

.PHONY: print
print:
	@echo INCLUDE_DIR: $(INCLUDE_DIR)
	@echo SOURCE_DIR: $(SOURCE_DIR)
	@echo OBJECT_DIR: $(OBJECT_DIR)
	@echo TARGET_DIR: $(TARGET_DIR)
	@echo SOURCES: $(SOURCES)
	@echo OBJECTS: $(OBJECTS)
	@echo DEPENDS: $(DEPENDS)
	@echo TARGET: $(TARGET)
	@echo LIBS: $(LIBS)
	@echo LOCAL_LIB_FILES: $(LOCAL_LIB_FILES)

-include $(DEPENDS)
	
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT) 
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

$(TARGET):	$(LOCAL_LIB_FILES) $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(LIBDIRS) -o $(TARGET) $(OBJECTS) $(LIBS)
