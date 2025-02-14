cmake_minimum_required (VERSION 3.15)

project (qemu-nrf52-wrapper LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS      ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(BUILD_DIR     "build")
set(MINGW_PREFIX  C:/ISISPACE/msys64)

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list "${CMAKE_CURRENT_SOURCE_DIR}/hw/*.h")
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)
list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")

#file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/arm/*.c")
#list(LENGTH SRCS SRCS_count)
#message(STATUS "[INFO] Found ${SRCS_count} source files.")

#add_definitions(-Wno-attributes -Wsign-compare -Wenum-compare -Werror)
add_definitions(-Werror=incompatible-pointer-types -Werror=unused-variable)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/hw/arm SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/hw/core SRCS)
list(LENGTH SRCS SRCS_count)
message(STATUS "[INFO] Found ${SRCS_count} source files.")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dtc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dtc/libfdt)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${BUILD_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${BUILD_DIR}/arm/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tcg/arm/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${BUILD_DIR}/arm-softmmu/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${BUILD_DIR}/hw/core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/target/arm)
include_directories(${header_dir_list})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hw/arm/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hw/adc)

include_directories(${MINGW_PREFIX}/mingw64/include)

include_directories(${MINGW_PREFIX}/mingw64/include/glib-2.0)
#include_directories(${MINGW_PREFIX}/mingw64/include/glib-2.0/include)
include_directories(${MINGW_PREFIX}/mingw64/lib/glib-2.0/include)

include_directories(${MINGW_PREFIX}/mingw64/include/pixman-1)
include_directories(${MINGW_PREFIX}/mingw64/lib/pixman-1/include)

include_directories(/usr/include/glib-2.0)
include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include)
include_directories(/usr/include/pixman-1)

add_definitions(-DNEED_CPU_H)
add_definitions(-DCONFIG_TARGET=\"arm-softmmu-config-target.h\")
add_definitions(-DCONFIG_DEVICES=\"arm-softmmu-config-devices.h\")

add_library(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/adc/nrf52_saadc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/dma/nrf52_edma.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/dma/nrf5x_ppi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/dma/nrf5x_radio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/char/nrf51_uart.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/timer/nrf_rtc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/timer/nrf51_timer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/timer/nrf_clock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/gpio/nrf52_gpiote.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/sensor/lsm6dsox.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/sensor/lis2dw12.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/sensor/tmp421.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/sensor/bmp390.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/adc/max11254.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/nvram/nrf51_nvm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hw/nvram/nrf52_nvm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/softmmu/memory.c
        ${SRCS})

# Build on Windows
include(ExternalProject)

find_program (MINGW_W64 mingw64.exe ${MINGW_PREFIX})
find_program (MINGW_BAT msys2_shell.cmd ${MINGW_PREFIX})

if (NOT EXISTS ${MINGW_PREFIX}/mingw64.exe)
    message(FATAL_ERROR "Please adjust your MINGW_PREFIX variable")
endif()

#message("MingW: ${MINGW_W64}")

set(MINGW_PATH "/mingw64/bin:/usr/local/bin:/usr/bin:/bin:/c/Windows/System32:/c/Windows:/c/Windows/System32/Wbem:/c/Windows/System32/WindowsPowerShell/v1.0/:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl")

ExternalProject_Add(qemu-nrf52
        DOWNLOAD_COMMAND ""
        SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/build
        CONFIGURE_COMMAND ""
#        BUILD_COMMAND cmd.exe /c set MSYSTEM=MINGW64 && ${MINGW_PREFIX}/usr/bin/bash --login -c "export PATH=${MINGW_PATH} && cd <SOURCE_DIR> &&pwd" # test command
        BUILD_COMMAND cmd.exe /c set MSYSTEM=MINGW64 && ${MINGW_PREFIX}/usr/bin/bash --login -c "export PATH=${MINGW_PATH} && cd <SOURCE_DIR> && make -j4 -k"
        BUILD_IN_SOURCE TRUE
        USES_TERMINAL_BUILD TRUE
        BUILD_ALWAYS    TRUE
        UPDATE_COMMAND    ""
        INSTALL_COMMAND   ""
        )
# BUILD_COMMAND ${MINGW_BAT} -mingw64 -c "export PATH=${ENV_PATH} && cd qemu-nrf52/build && make -j4 -k" # also works, less practical
add_dependencies(${PROJECT_NAME} qemu-nrf52)

file( GENERATE OUTPUT "${CMAKE_BINARY_DIR}/.gitignore" CONTENT "*" )
