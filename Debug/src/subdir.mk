################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Application.cpp \
../src/CommandLineView.cpp \
../src/Document.cpp \
../src/Irrigation.cpp \
../src/Program.cpp \
../src/Valve.cpp \
../src/View.cpp \
../src/WateringSpecified.cpp 

OBJS += \
./src/Application.o \
./src/CommandLineView.o \
./src/Document.o \
./src/Irrigation.o \
./src/Program.o \
./src/Valve.o \
./src/View.o \
./src/WateringSpecified.o 

CPP_DEPS += \
./src/Application.d \
./src/CommandLineView.d \
./src/Document.d \
./src/Irrigation.d \
./src/Program.d \
./src/Valve.d \
./src/View.d \
./src/WateringSpecified.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++14 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


