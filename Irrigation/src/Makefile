
TARGET_NAME     = Irrigation

########################################################
########################################################

PROJECT_ROOT    = ..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/src
OBJECT_DIR      = $(PROJECT_ROOT)/build/src
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/bin
LIBRARY_DIR      = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/$(TARGET_NAME).exe
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
DEPENDS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.d))

########################################################
########################################################

LOCAL_LIBS      = 
LOCAL_LIB_FILES = $(patsubst -l%,$(LIBRARY_DIR)/lib%.a,$(LOCAL_LIBS))
INCLUDE_PATH    = -I$(INCLUDE_DIR)
LIBDIRS         = -L$(LIBRARY_DIR) -L/usr/local/lib
LIBS            = -lteng -lmicrohttpd $(LOCAL_LIBS)

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0 -MMD -MP  
CPPFLAGS        = $(INCLUDE_PATH) 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3


.PHONY: all
all: library $(TARGET)

.PHONY: clean
clean: library-clean

.PHONY: library
library:
	@$(MAKE) -C CommandExecutor library
	@$(MAKE) -C DocumentView library
	@$(MAKE) -C Hardware library
	@$(MAKE) -C Logger library
	@$(MAKE) -C Logic library
	@$(MAKE) -C Model library
	@$(MAKE) -C Schedulers library
	#@$(MAKE) -C Utils library
	#@$(MAKE) -C Views/CommandLineView library
	#@$(MAKE) -C Views/WebServerView library
	#@$(MAKE) -C WebServer library
	
.PHONY: library-clean
library-clean:
	@$(MAKE) -C CommandExecutor library-clean
	@$(MAKE) -C DocumentView library-clean
	@$(MAKE) -C Hardware library-clean
	@$(MAKE) -C Logger library-clean
	@$(MAKE) -C Logic library-clean
	@$(MAKE) -C Model library-clean
	@$(MAKE) -C Schedulers library-clean
	#@$(MAKE) -C Utils library-clean
	#@$(MAKE) -C Views/CommandLineView library-clean
	#@$(MAKE) -C Views/WebServerView library-clean
	#@$(MAKE) -C WebServer library-clean
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
	@echo LOCAL_LIB_FILES: $(LOCAL_LIB_FILES)

-include $(DEPENDS)
	
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	
$(TARGET):	$(LOCAL_LIB_FILES) $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(LIBDIRS) -o $(TARGET) $(OBJECTS) $(LIBS)
