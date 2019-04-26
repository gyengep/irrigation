
TARGET_NAME     = $(shell basename $(CURDIR))

########################################################
########################################################

PROJECT_ROOT    = ../../..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/lib/Views/$(TARGET_NAME)
OBJECT_DIR      = $(PROJECT_ROOT)/build/lib/Views/$(TARGET_NAME)
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/lib$(TARGET_NAME).a
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
DEPENDS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.d))

########################################################
########################################################

INCLUDE_PATH    = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/Views/$(TARGET_NAME) -I$(PROJECT_ROOT)/3p/pugixml

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0 -MMD -MP 
CPPFLAGS        = $(INCLUDE_PATH) 
ARFLAGS         = r 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3


.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
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

-include $(DEPENDS)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT) 
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	
$(TARGET):	$(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)
	