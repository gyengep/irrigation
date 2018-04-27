
TARGET_NAME     = CommandLineView

########################################################
########################################################

PROJECT_ROOT    = ../../..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/test/$(TARGET_NAME)
OBJECT_DIR      = $(PROJECT_ROOT)/build/test/$(TARGET_NAME)
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/$(TARGET_NAME)_test.exe
SOURCES         = $(shell find $(SOURCE_DIR) -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))

########################################################
########################################################

INCLUDE_PATH    = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/$(TARGET_NAME)
LIBDIRS         = -L$(TARGET_DIR)
LIBS            = -lgmock -l$(TARGET_NAME) 

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0
CPPFLAGS        = -isystem $(PROJECT_ROOT)/lib/gmock_gtest $(INCLUDE_PATH) 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3


all: test

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OBJECT_DIR)
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo 'Finished building: $<'
	@echo ' '

$(TARGET): $(OBJECTS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cygwin C++ Linker'
	$(CXX) $(LIBDIRS) -o $(TARGET) $(OBJECTS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

clean:	
	rm -f $(OBJECTS)
	rm -f $(TARGET)

print:
	@echo INCLUDE_DIR: $(INCLUDE_DIR)
	@echo SOURCE_DIR: $(SOURCE_DIR)
	@echo OBJECT_DIR: $(OBJECT_DIR)
	@echo TARGET_DIR: $(TARGET_DIR)
	@echo SOURCES: $(SOURCES)
	@echo OBJECTS: $(OBJECTS)
	@echo TARGET: $(TARGET)

test: $(TARGET)
	@echo 'Executing test: $(TARGET)'
	@$(TARGET)
	@echo 'Finished executing test: $(TARGET)'
	@echo ' ' 

