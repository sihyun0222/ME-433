cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(homework4 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_executable(homework4
        homework4.c
        )

# pull in common dependencies
target_link_libraries(homework4 pico_stdlib hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(homework4)
pico_enable_stdio_usb(homework4 1) 
pico_enable_stdio_uart(homework4 0)



