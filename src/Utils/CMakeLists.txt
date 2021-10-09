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
    dlist.c
    dlist.h
    symbolelement.c
    symbolelement.h
    logger.h
    )

add_library(${PROJECT_NAME} STATIC ${UTILITY_FILES})

set_target_properties( ${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/${PROJECT_NAME})

if (MSVC)
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
