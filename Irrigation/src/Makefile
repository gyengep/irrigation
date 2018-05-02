
TARGET_NAME     = Irrigation

########################################################
########################################################

PROJECT_ROOT    = ..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/src
OBJECT_DIR      = $(PROJECT_ROOT)/build/src
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/bin
LOCAL_LIB_DIR   = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/$(TARGET_NAME).exe
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
DEPENDS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.d))

########################################################
########################################################

INCLUDE_PATH    = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/$(TARGET_NAME)
LIBDIRS         = -L$(LOCAL_LIB_DIR) -L/usr/local/lib
LIBS            = -lCommons -lModel -lHardware -lCommandLineView -lCommand -lWebServerView -lWebServer -lLogic -lUtils -lDocumentView -lteng -lmicrohttpd

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0 -MMD -MP  
CPPFLAGS        = $(INCLUDE_PATH) 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3


all: dependencies $(TARGET)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	
$(TARGET):	$(OBJECTS)
	$(CXX) $(LIBDIRS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean: 
	@$(MAKE) -C Command clean
	@$(MAKE) -C Commons clean
	@$(MAKE) -C DocumentView clean
	@$(MAKE) -C Hardware clean
	@$(MAKE) -C Logic clean
	@$(MAKE) -C Model clean
	@$(MAKE) -C Utils clean
	@$(MAKE) -C Views/CommandLineView clean
	@$(MAKE) -C Views/WebServerView clean
	@$(MAKE) -C WebServer clean
	rm -f $(OBJECTS)
	rm -f $(DEPENDS)
	rm -f $(TARGET)

print:
	@echo INCLUDE_DIR: $(INCLUDE_DIR)
	@echo SOURCE_DIR: $(SOURCE_DIR)
	@echo OBJECT_DIR: $(OBJECT_DIR)
	@echo TARGET_DIR: $(TARGET_DIR)
	@echo SOURCES: $(SOURCES)
	@echo OBJECTS: $(OBJECTS)
	@echo DEPENDS: $(DEPENDS)
	@echo TARGET: $(TARGET)

dependencies:
	@$(MAKE) -C Command all
	@$(MAKE) -C Commons all
	@$(MAKE) -C DocumentView all
	@$(MAKE) -C Hardware all
	@$(MAKE) -C Logic all
	@$(MAKE) -C Model all
	@$(MAKE) -C Utils all
	@$(MAKE) -C Views/CommandLineView all
	@$(MAKE) -C Views/WebServerView all
	@$(MAKE) -C WebServer all
	
test:
	@$(MAKE) -C ../test/Command all
	@$(MAKE) -C ../test/Commons all
	#@$(MAKE) -C ../test/DocumentView all
	#@$(MAKE) -C ../test/Hardware all
	@$(MAKE) -C ../test/Logic all
	#@$(MAKE) -C ../test/Model all
	@$(MAKE) -C ../test/Utils all
	@$(MAKE) -C ../Views/test/CommandLineView all
	#@$(MAKE) -C ../Views/test/WebServerView all
	#@$(MAKE) -C ../test/WebServer all

test-clean:
	@$(MAKE) -C ../test/Command clean
	@$(MAKE) -C ../test/Commons clean
	#@$(MAKE) -C ../test/DocumentView clean
	#@$(MAKE) -C ../test/Hardware clean
	@$(MAKE) -C ../test/Logic clean
	#@$(MAKE) -C ../test/Model clean
	@$(MAKE) -C ../test/Utils clean
	@$(MAKE) -C ../test/Views/CommandLineView clean
	#@$(MAKE) -C ../test/Views/WebServerView clean
	#@$(MAKE) -C ../test/WebServer clean
	
-include $(DEPENDS)
