cmake_minimum_required(VERSION 3.8.0)

project(vpr-bin2fmt C)
set(TARGET vpr-bin2fmt)

set(CMAKE_C_STANDARD 99)

set(TARGET_SOURCE_DIR src)
set(TARGET_SOURCES
    ${TARGET_SOURCE_DIR}/main.c
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${TARGET} ${TARGET_SOURCES})
