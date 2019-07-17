
TARGET_NAME     = irrigation

########################################################
########################################################

PROJECT_ROOT    = ..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/src
OBJECT_DIR      = $(PROJECT_ROOT)/build/src
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/bin
LIBRARY_DIR     = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/$(TARGET_NAME)
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
DEPENDS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.d))

########################################################
########################################################

LOCAL_LIBS      = -lLogger -lModel -lDocumentView -lLogic -lSchedulers -lDTO -lTimerView -lValves -lRestView -lWebServer -lTemperature -lUtils -lDtoReaderWriter
INCLUDES	    = -I$(INCLUDE_DIR)
LIBDIRS         = -L$(LIBRARY_DIR) -L/usr/local/lib
LIBS            = $(LOCAL_LIBS) -pthread -lpugixml -lmicrohttpd -lcurl -lssl -lcrypto

# Detect the Raspberry Pi from cpuinfo
ifneq "$(shell cat /proc/cpuinfo | grep Hardware | grep -c BCM)" "0"
LIBS          += -lwiringPi
endif

include ../common.mk

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
	@echo LIBS: $(LIBS)

-include $(DEPENDS)
	
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
	
$(TARGET):	$(patsubst -l%,$(LIBRARY_DIR)/lib%.a,$(LOCAL_LIBS)) $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(LIBDIRS) -o $(TARGET) $(OBJECTS) $(LIBS)
