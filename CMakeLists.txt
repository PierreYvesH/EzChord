# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD adafruit_feather_rp2040 CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.20)

# Pico SDK (before project)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# Project
project(EzChord VERSION 0.1)
set(CMAKE_C_STANDARD 11)
pico_sdk_init()

# Source
add_executable(EzChord
    EzChord/led.c
    EzChord/main.c
    EzChord/matrix.c
    EzChord/usb_descriptors.c)
target_link_libraries(EzChord pico_stdlib pico_unique_id tinyusb_device)
target_include_directories(EzChord PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(
    EzChord PRIVATE CFG_TUSB_CONFIG_FILE="EzChord/tusb_config.h")

# Binary
pico_set_program_version(EzChord "${PROJECT_VERSION}")
pico_set_program_description(EzChord "EzChord firmware")
pico_set_binary_type(EzChord copy_to_ram)
pico_add_extra_outputs(EzChord)

# Verbose compiler warnings
if(MSVC)
    target_compile_options(EzChord PRIVATE /W4 /WX)
else()
    target_compile_options(EzChord PRIVATE -Wall -Wextra -Werror)
endif()
