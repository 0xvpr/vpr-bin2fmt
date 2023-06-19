cmake_minimum_required( VERSION 3.22 )

project( bin2fmt C )

set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_C_STANDARD 99 )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

set( SOURCE_DIR ${CMAKE_SOURCE_DIR}/src )
set( SOURCES
    # C Sources
    ${SOURCE_DIR}/main.c
)

add_executable( ${PROJECT_NAME} ${SOURCES} )
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        PREFIX vpr-
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE $<$<C_COMPILER_ID:MSVC>:/W4>
            $<$<C_COMPILER_ID:MSVC>:/O2>
            $<$<C_COMPILER_ID:MSVC>:/DNDEBUG>
            $<$<C_COMPILER_ID:GNU>:-Ofast>
            $<$<C_COMPILER_ID:GNU>:-Wall>
            $<$<C_COMPILER_ID:GNU>:-Wextra>
            $<$<C_COMPILER_ID:GNU>:-Werror>
            $<$<C_COMPILER_ID:GNU>:-Wshadow>
            $<$<C_COMPILER_ID:GNU>:-Wpedantic>
            $<$<C_COMPILER_ID:GNU>:-Wconversion>
            $<$<C_COMPILER_ID:GNU>:-ffunction-sections>
            $<$<C_COMPILER_ID:GNU>:-ffast-math>
            $<$<C_COMPILER_ID:GNU>:-funroll-loops>
            $<$<C_COMPILER_ID:GNU>:-fPIE>
)

target_link_options(
    ${PROJECT_NAME}
    PRIVATE $<$<C_COMPILER_ID:MSVC>:/MANIFEST:NO>
            $<$<C_COMPILER_ID:GNU>:-s>
)

install(
    TARGETS     ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
