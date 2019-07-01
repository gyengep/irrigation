
TARGET_NAME     = pugixml

########################################################
########################################################

PROJECT_ROOT    = ../..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/3p/pugixml
OBJECT_DIR      = $(PROJECT_ROOT)/build/3p/pugixml
INCLUDE_DIR     = $(PROJECT_ROOT)/3p/pugixml
TARGET_DIR      = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/lib$(TARGET_NAME).a
SOURCES         = $(shell find $(SOURCE_DIR) -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))

########################################################
########################################################

INCLUDES    = -I$(INCLUDE_DIR)
ARFLAGS         = -r
CXXFLAGS        = -O3 -Wall -fmessage-length=0

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
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
	@echo 'Finished building: $<'
	@echo ' '

$(TARGET):	$(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	$(AR) $(ARFLAGS) $@ $(OBJECTS)
	@echo 'Finished building target: $@'
	@echo ' '
