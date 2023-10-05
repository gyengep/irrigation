
TARGET_NAME     = $(shell basename $(CURDIR))

########################################################
########################################################

PROJECT_ROOT    = ../../..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/test/Views/$(TARGET_NAME)
COMMON_DIR      = $(PROJECT_ROOT)/test/Common
OBJECT_DIR      = $(PROJECT_ROOT)/build/test/$(TARGET_NAME)
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/build
LIBRARY_DIR     = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/$(TARGET_NAME)_test
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
DEPENDS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.d))

########################################################
########################################################

LOCAL_LIBS      = -l$(TARGET_NAME) -lModel -lLogger -lWebServer -lDtoReaderWriter -lDocumentView -lLogic -lSchedulers -lValves -lTemperature -lDS18B20 -lEmail -lUtils
INCLUDES	    = -isystem $(PROJECT_ROOT)/3p/gmock_gtest -I$(INCLUDE_DIR) -I$(COMMON_DIR)
LIBDIRS         = -L$(LIBRARY_DIR) -L/usr/local/lib 
LIBS            = $(LOCAL_LIBS) -pthread -lgmock -lpugixml -lmicrohttpd -lcurl -lssl -lcrypto -ldl

# Detect the Raspberry Pi from cpuinfo
# Count the matches for BCMxxxx in cpuinfo
RPI            = $(shell cat /proc/cpuinfo | grep Hardware | grep -c BCM)

ifeq "$(RPI)" "1"
LIBS          += -lwiringPi
endif

include ../../../common.mk

.PHONY: all
all: $(TARGET)
	@echo 'Executing test: $(TARGET)'
	@$(TARGET)
	@echo 'Finished executing test: $(TARGET)'
	@echo ' '

.PHONY: clean
clean: 
	rm -f $(OBJECTS)
	rm -f $(DEPENDS)
	rm -f $(TARGET)

.PHONY: all-with-libraries
all-with-libraries: library-all all

.PHONY: clean-with-libraries
clean-with-libraries: library-clean clean
		
.PHONY: library-all
library-all: $(patsubst -l%,%-all,$(LOCAL_LIBS))

.PHONY: library-clean
library-clean: $(patsubst -l%,%-clean,$(LOCAL_LIBS))

.PHONY: $(patsubst -l%,%-all,$(LOCAL_LIBS))
$(patsubst -l%,%-all,$(LOCAL_LIBS)):
	@$(MAKE) -C $(PROJECT_ROOT)/lib $@

.PHONY: $(patsubst -l%,%-clean,$(LOCAL_LIBS))
$(patsubst -l%,%-clean,$(LOCAL_LIBS)):
	@$(MAKE) -C $(PROJECT_ROOT)/lib $@
	
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

-include $(DEPENDS)
	
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT) 
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(TARGET):	$(patsubst -l%,$(LIBRARY_DIR)/lib%.a,$(LOCAL_LIBS)) $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(LIBDIRS) -o $(TARGET) $(OBJECTS) $(LIBS)
