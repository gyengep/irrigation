
TARGET_NAME     = Irrigation

########################################################
########################################################

PROJECT_ROOT    = ..
SRC_EXT         = cpp
SOURCE_DIR      = $(PROJECT_ROOT)/src
OBJECT_DIR      = $(PROJECT_ROOT)/build/src
INCLUDE_DIR     = $(PROJECT_ROOT)/include
TARGET_DIR      = $(PROJECT_ROOT)/build

TARGET          = $(TARGET_DIR)/$(TARGET_NAME).exe
SOURCES         = $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name '*.$(SRC_EXT)')
OBJECTS         = $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))

########################################################
########################################################

INCLUDE_PATH    = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/$(TARGET_NAME)
LIBDIRS         = -L$(TARGET_DIR) -L/usr/local/lib
LIBS            = -lCommons -lModel -lHardware -lCommandLineView -lCommand -lWebServerView -lWebServer -lLogic -lUtils -lDocumentView -lteng -lmicrohttpd

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0
CPPFLAGS        = $(INCLUDE_PATH) 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3


all: dependencies $(TARGET)

dependencies:
	$(MAKE) -C Command all
	$(MAKE) -C Commons all
	$(MAKE) -C DocumentView all
	$(MAKE) -C Hardware all
	$(MAKE) -C Logic all
	$(MAKE) -C Model all
	$(MAKE) -C Utils all
	$(MAKE) -C Views/CommandLineView all
	$(MAKE) -C Views/WebServerView all
	$(MAKE) -C WebServer all

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OBJECT_DIR)
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo 'Finished building: $<'
	@echo ' '
	
$(TARGET):	$(OBJECTS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cygwin C++ Linker'
	$(CXX) $(LIBDIRS) -o $(TARGET) $(OBJECTS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

clean: 
	$(MAKE) -C Command clean
	$(MAKE) -C Commons clean
	$(MAKE) -C DocumentView clean
	$(MAKE) -C Hardware clean
	$(MAKE) -C Logic clean
	$(MAKE) -C Model clean
	$(MAKE) -C Utils clean
	$(MAKE) -C Views/CommandLineView clean
	$(MAKE) -C Views/WebServerView clean
	$(MAKE) -C WebServer clean
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

test:
	$(MAKE) -C Command test
	$(MAKE) -C Commons test
	$(MAKE) -C DocumentView test
	$(MAKE) -C Hardware test
	$(MAKE) -C Logic test
	$(MAKE) -C Model test
	$(MAKE) -C Utils test
	$(MAKE) -C Views/CommandLineView test
	$(MAKE) -C Views/WebServerView test
	$(MAKE) -C WebServer test
