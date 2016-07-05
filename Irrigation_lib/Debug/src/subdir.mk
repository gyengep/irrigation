################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Application.cpp \
../src/CommandLineView.cpp \
../src/DaySchedulerSpecified.cpp \
../src/Document.cpp \
../src/Program.cpp \
../src/Valve.cpp \
../src/View.cpp 

OBJS += \
./src/Application.o \
./src/CommandLineView.o \
./src/DaySchedulerSpecified.o \
./src/Document.o \
./src/Program.o \
./src/Valve.o \
./src/View.o 

CPP_DEPS += \
./src/Application.d \
./src/CommandLineView.d \
./src/DaySchedulerSpecified.d \
./src/Document.d \
./src/Program.d \
./src/Valve.d \
./src/View.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


