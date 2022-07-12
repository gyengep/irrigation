
TARGET_NAME     = $(shell basename $(CURDIR))

########################################################
########################################################

PROJECT_ROOT    = ../..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/lib/$(TARGET_NAME)
OBJECT_DIR      = $(PROJECT_ROOT)/build/lib/$(TARGET_NAME)
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/lib$(TARGET_NAME).a
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
DEPENDS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.d))

########################################################
########################################################

INCLUDES            = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/$(TARGET_NAME)
ARFLAGS             = r 
SENDINBLUE_SMTP_KEY = $(shell printenv SENDINBLUE_SMTP_KEY)

ifndef SENDINBLUE_SMTP_KEY
$(error Error! SENDINBLUE_SMTP_KEY is not set)
endif

include ../../common.mk

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
	@echo SENDINBLUE_SMTP_KEY: $(SENDINBLUE_SMTP_KEY)

-include $(DEPENDS)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT) 
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $< -Dsendinblue_smtp_key=\"$(SENDINBLUE_SMTP_KEY)\"
	
$(TARGET):	$(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)
	