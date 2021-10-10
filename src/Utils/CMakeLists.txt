cmake_minimum_required(VERSION 3.15)

project(Utils)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)


set(UTILITY_FILES
    Token.c
    Token.h
    symtable.c
    symtable.h
    hashtable.c
    hashtable.h
    "list.c"
    "list.h"
    "listelement.c"
    "listelement.h"
    stack.c
    stack.h
    symbolelement.c
    symbolelement.h
    logger.h
    )

add_library(${PROJECT_NAME} STATIC ${UTILITY_FILES})

if (MSVC)
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

else()
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/$(Configuration))
endif()
