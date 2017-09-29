################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../HAL_I2C.cpp \
../HAL_OPT3001.cpp \
../main.cpp 

LDS_SRCS += \
../msp432p401r.lds 

C_SRCS += \
../startup_msp432p401r_gcc.c \
../system_msp432p401r.c 

C_DEPS += \
./startup_msp432p401r_gcc.d \
./system_msp432p401r.d 

OBJS += \
./HAL_I2C.o \
./HAL_OPT3001.o \
./main.o \
./startup_msp432p401r_gcc.o \
./system_msp432p401r.o 

CPP_DEPS += \
./HAL_I2C.d \
./HAL_OPT3001.d \
./main.d 

OBJS__QUOTED += \
"HAL_I2C.o" \
"HAL_OPT3001.o" \
"main.o" \
"startup_msp432p401r_gcc.o" \
"system_msp432p401r.o" 

C_DEPS__QUOTED += \
"startup_msp432p401r_gcc.d" \
"system_msp432p401r.d" 

CPP_DEPS__QUOTED += \
"HAL_I2C.d" \
"HAL_OPT3001.d" \
"main.d" 

CPP_SRCS__QUOTED += \
"../HAL_I2C.cpp" \
"../HAL_OPT3001.cpp" \
"../main.cpp" 

C_SRCS__QUOTED += \
"../startup_msp432p401r_gcc.c" \
"../system_msp432p401r.c" 


