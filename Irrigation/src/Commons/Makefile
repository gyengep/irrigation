
TARGET_NAME     = Commons

########################################################
########################################################

PROJECT_ROOT    = ../..
SOURCE_DIR      = $(PROJECT_ROOT)/src/$(TARGET_NAME)
OBJECT_DIR      = $(PROJECT_ROOT)/build/src/$(TARGET_NAME)
TEST_SOURCE_DIR = $(PROJECT_ROOT)/test/$(TARGET_NAME)
TEST_OBJECT_DIR = $(PROJECT_ROOT)/build/test/$(TARGET_NAME)
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/build
GMOCK_DIR       = $(PROJECT_ROOT)/lib/gmock_gtest

TARGET          = $(TARGET_DIR)/lib$(TARGET_NAME).a
TEST_TARGET     = $(TARGET_DIR)/$(TARGET_NAME)_test.exe

########################################################
########################################################

SOURCES         = $(shell find $(SOURCE_DIR) -type f -name '*.cpp')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.cpp=.o))
TEST_SOURCES    = $(shell find $(TEST_SOURCE_DIR) -type f -name '*.cpp')
TEST_OBJECTS    = $(patsubst $(TEST_SOURCE_DIR)/%,$(TEST_OBJECT_DIR)/%,$(TEST_SOURCES:.cpp=.o))

########################################################
########################################################

INCLUDE_PATH    = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/$(TARGET_NAME)
LIB_PATH        = -L$(TARGET_DIR)
LIBS            = -lgmock 
LIBS            += -l$(TARGET_NAME) 

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0
CPPFLAGS        = -isystem $(GMOCK_DIR) $(INCLUDE_PATH) 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3


$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(OBJECT_DIR)
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo 'Finished building: $<'
	@echo ' '

$(TEST_OBJECT_DIR)/%.o: $(TEST_SOURCE_DIR)/%.cpp
	@mkdir -p $(TEST_OBJECT_DIR)
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo 'Finished building: $<'
	@echo ' '

	
$(TARGET):	$(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	$(AR) $(ARFLAGS) $@ $(OBJECTS)
	@echo 'Finished building target: $@'
	@echo ' '

$(TEST_TARGET): $(TEST_OBJECTS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cygwin C++ Linker'
	$(CXX) $(LIB_PATH) -o $(TEST_TARGET) $(TEST_OBJECTS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

all: $(TARGET)

clean:
	rm -rf $(OBJECT_DIR)
	rm -rf $(TEST_OBJECT_DIR)
	rm -f $(TARGET)
	rm -f $(TEST_TARGET)
	
test: $(TARGET) $(TEST_TARGET)
	@echo 'Executing test: $(TEST_TARGET)'
	@$(TEST_TARGET)
	@echo 'Finished executing test: $(TEST_TARGET)'
	@echo ' '

print:
	@echo INCLUDE_DIR: $(INCLUDE_DIR)
	@echo SOURCE_DIR: $(SOURCE_DIR)
	@echo TEST_SOURCE_DIR: $(TEST_SOURCE_DIR)
	@echo OBJECT_DIR: $(OBJECT_DIR)
	@echo TEST_OBJECT_DIR: $(TEST_OBJECT_DIR)
	@echo SOURCES: $(SOURCES)
	@echo OBJECTS: $(OBJECTS)
	@echo TEST_SOURCES: $(TEST_SOURCES)
	@echo TEST_OBJECTS: $(TEST_OBJECTS)
	@echo TARGET_DIR: $(TARGET_DIR)
	@echo TARGET: $(TARGET)
	@echo TEST_TARGET: $(TEST_TARGET)
