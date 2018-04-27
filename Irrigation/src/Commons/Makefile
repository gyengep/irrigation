
TARGET_NAME     = Commons

########################################################
########################################################

PROJECT_ROOT    = ../..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/src/$(TARGET_NAME)
OBJECT_DIR      = $(PROJECT_ROOT)/build/src/$(TARGET_NAME)
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/build
TEST_DIR        = $(PROJECT_ROOT)/test/$(TARGET_NAME)

TARGET          = $(TARGET_DIR)/lib$(TARGET_NAME).a
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))

########################################################
########################################################

INCLUDE_PATH    = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/$(TARGET_NAME)
LIBDIRS         = 
LIBS            = 

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0
CPPFLAGS        = $(INCLUDE_PATH) 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3


all: $(TARGET)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OBJECT_DIR)
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

clean: 
	$(MAKE) -C $(TEST_DIR) clean
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
	$(MAKE) -C $(TEST_DIR) test
