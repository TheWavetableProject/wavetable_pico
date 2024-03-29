cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(my_proj)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if (TARGET tinyusb_device)
    # rest of your project
    add_executable(my_proj
        main.pio
        build/out.c
    )


    # PIO things
    # generate the header file into the source tree as it is included in the RP2040 datasheet
    pico_generate_pio_header(my_proj ${CMAKE_CURRENT_LIST_DIR}/main.pio)
    target_sources(my_proj PRIVATE build/out.c)

    # Add pico_stdlib library which aggregates commonly used features
    target_link_libraries(my_proj pico_stdlib pico_multicore pico_time hardware_pio)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(my_proj 1)
    pico_enable_stdio_uart(my_proj 0)

    # create map/bin/hex/uf2 file in addition to ELF.
    pico_add_extra_outputs(my_proj)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()

