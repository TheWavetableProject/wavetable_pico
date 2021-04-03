cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(my_proj
    main.c
    main.pio
)

# PIO things
# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(my_proj ${CMAKE_CURRENT_LIST_DIR}/main.pio)

target_sources(my_proj PRIVATE main.c)

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(my_proj pico_stdlib pico_multicore pico_time hardware_pio)

# enable usb output, disable uart output
pico_enable_stdio_usb(my_proj 1)
pico_enable_stdio_uart(my_proj 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(my_proj)

