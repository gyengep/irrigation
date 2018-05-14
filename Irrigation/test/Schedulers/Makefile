
TARGET_NAME     = $(shell basename $(CURDIR))

########################################################
########################################################

PROJECT_ROOT    = ../..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/test/$(TARGET_NAME)
OBJECT_DIR      = $(PROJECT_ROOT)/build/test/$(TARGET_NAME)
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/build
UNDER_TEST_DIR  = $(PROJECT_ROOT)/src/$(TARGET_NAME)

TARGET          = $(TARGET_DIR)/$(TARGET_NAME)_test.exe
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
DEPENDS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.d))

########################################################
########################################################

INCLUDE_PATH    = -I$(INCLUDE_DIR)
LIBDIRS         = -L$(TARGET_DIR)
LOCAL_LIB_NAMES = $(TARGET_NAME)
LIBS            = -lgmock $(patsubst %,-l%,$(LOCAL_LIB_NAMES))

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0 -MMD -MP 
CPPFLAGS        = -isystem $(PROJECT_ROOT)/lib/gmock_gtest $(INCLUDE_PATH) 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3


all: library test

clean: library-clean test-clean

-include $(DEPENDS)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT) 
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

$(TARGET):	$(OBJECTS) $(patsubst %,$(TARGET_DIR)/lib%.a,$(LOCAL_LIB_NAMES))
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(LIBDIRS) -o $(TARGET) $(OBJECTS) $(LIBS)

	
library:
	@$(MAKE) -C $(UNDER_TEST_DIR) library

library-clean:
	@$(MAKE) -C $(UNDER_TEST_DIR) library-clean

test: $(TARGET)
	@echo 'Executing test: $(TARGET)'
	@$(TARGET)
	@echo 'Finished executing test: $(TARGET)'
	@echo ' ' 

test-clean:
	rm -f $(OBJECTS)
	rm -f $(DEPENDS)
	rm -f $(TARGET)

print:
	@echo INCLUDE_DIR: $(INCLUDE_DIR)
	@echo SOURCE_DIR: $(SOURCE_DIR)
	@echo OBJECT_DIR: $(OBJECT_DIR)
	@echo TARGET_DIR: $(TARGET_DIR)
	@echo UNDER_TEST_DIR: $(UNDER_TEST_DIR)
	@echo SOURCES: $(SOURCES)
	@echo OBJECTS: $(OBJECTS)
	@echo DEPENDS: $(DEPENDS)
	@echo LOCAL_LIB_NAMES: $(LOCAL_LIB_NAMES)
	@echo LIBS: $(LIBS)
	@echo TARGET: $(TARGET)
