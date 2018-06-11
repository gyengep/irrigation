
TARGET_NAME     = gmock

########################################################
########################################################

PROJECT_ROOT    = ../..
SRC_EXT         = cc
SOURCE_DIR      = $(PROJECT_ROOT)/3p/gmock_gtest
OBJECT_DIR      = $(PROJECT_ROOT)/build/3p/gmock_gtest
INCLUDE_DIR     = $(PROJECT_ROOT)/3p/gmock_gtest
TARGET_DIR      = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/lib$(TARGET_NAME).a
SOURCES         = $(shell find $(SOURCE_DIR) -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))

########################################################
########################################################

INCLUDE_PATH    = -I$(INCLUDE_DIR)

ARFLAGS         = -r
CXXFLAGS        = -O3 -Wall -fmessage-length=0
CPPFLAGS        =  $(INCLUDE_PATH) 


.PHONY:  all
all: $(TARGET)

.PHONY:  clean
clean:
	rm -rf $(OBJECT_DIR)
	rm -f $(TARGET)

.PHONY:  print
print:
	@echo INCLUDE_DIR: $(INCLUDE_DIR)
	@echo SOURCE_DIR: $(SOURCE_DIR)
	@echo OBJECT_DIR: $(OBJECT_DIR)
	@echo SOURCES: $(SOURCES)
	@echo OBJECTS: $(OBJECTS)
	@echo TARGET_DIR: $(TARGET_DIR)
	@echo TARGET: $(TARGET)
	
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
