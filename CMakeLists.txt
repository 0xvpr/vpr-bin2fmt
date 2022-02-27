cmake_minimum_required(VERSION 3.8.0)

project(vpr-bin2fmt C)
set(TARGET vpr-bin2fmt)

set(CMAKE_C_STANDARD 17)

set(TARGET_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(TARGET_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(TARGET_SOURCES
    ${TARGET_SOURCE_DIR}/main.c
    ${TARGET_SOURCE_DIR}/parser.c
    ${TARGET_SOURCE_DIR}/util.c
    ${TARGET_INCLUDE_DIR}/parser.h
    ${TARGET_INCLUDE_DIR}/util.h
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${TARGET} ${TARGET_INCLUDE_DIR})

add_executable(${TARGET} ${TARGET_SOURCES})
