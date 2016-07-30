################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Application.cpp \
../src/CommandLineView.cpp \
../src/DaySchedulerSpecified.cpp \
../src/Document.cpp \
../src/Irrigation.cpp \
../src/Program.cpp \
../src/Valve.cpp \
../src/View.cpp 

OBJS += \
./bin/src/Application.o \
./bin/src/CommandLineView.o \
./bin/src/DaySchedulerSpecified.o \
./bin/src/Document.o \
./bin/src/Irrigation.o \
./bin/src/Program.o \
./bin/src/Valve.o \
./bin/src/View.o 

CPP_DEPS += \
./bin/src/Application.d \
./bin/src/CommandLineView.d \
./bin/src/DaySchedulerSpecified.d \
./bin/src/Document.d \
./bin/src/Irrigation.d \
./bin/src/Program.d \
./bin/src/Valve.d \
./bin/src/View.d 


# Each subdirectory must supply rules for building sources it contributes
bin/src/%.o: ./src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


