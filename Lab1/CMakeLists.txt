cmake_minimum_required(VERSION 3.20)
project(Lab1 C CXX ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Define the ARM cross-compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# Define compilation flags
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -Wall -ffunction-sections -fdata-sections" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-T path/to/your/linker_script.ld -Wl,--gc-sections" CACHE STRING "" FORCE)

# Include CMSIS and TivaWare libraries if needed
include_directories(
        ${CMAKE_SOURCE_DIR}/inc
        ${CMAKE_SOURCE_DIR}/driverlib
)

# Source files
file(GLOB_RECURSE SOURCES "src/*.c" "driverlib/*.c")

add_executable(Lab1 ${SOURCES})
