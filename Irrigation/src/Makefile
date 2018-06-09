
TARGET_NAME     = Irrigation

########################################################
########################################################

PROJECT_ROOT    = ..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/src
OBJECT_DIR      = $(PROJECT_ROOT)/build/src
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/bin
LIBRARY_DIR     = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/$(TARGET_NAME).exe
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
DEPENDS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.d))

########################################################
########################################################

LOCAL_LIBS      = -lLogger -lModel -lHardware -lDocumentView -lLogic -lSchedulers	
INCLUDE_PATH    = -I$(INCLUDE_DIR)
LIBDIRS         = -L$(LIBRARY_DIR) -L/usr/local/lib
LIBS            = $(LOCAL_LIBS) -lteng -lmicrohttpd

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0 -MMD -MP  
CPPFLAGS        = $(INCLUDE_PATH) 

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
	@echo LIBS: $(LIBS)

-include $(DEPENDS)
	
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	
$(TARGET):	$(patsubst -l%,$(LIBRARY_DIR)/lib%.a,$(LOCAL_LIBS)) $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(LIBDIRS) -o $(TARGET) $(OBJECTS) $(LIBS)