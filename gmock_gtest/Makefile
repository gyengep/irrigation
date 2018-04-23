# Source files

SOURCES = \
	gmock-gtest-all.cc

LIBS = \

TARGET = libgmock.a



OBJS = $(SOURCES:%.cc=%.o)
DEPS = $(SOURCES:%.cc=%.d)

ARFLAGS = -r
CXXFLAGS = -I. -O3 -Wall -fmessage-length=0 -c


%.o: %.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	$(CXX) $(CXXFLAGS) -o $@ $< 
	@echo 'Finished building: $<'
	@echo ' '

$(TARGET):	$(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

all:	$(TARGET)

clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET)
	@echo ' '
