cmake_minimum_required(VERSION 3.15)

project(Utils)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)


set(UTILITY_FILES
    Token.c
    Token.h
    symtable_testonly.h
    symtable.c
    symtable.h
    hashtable.c
    hashtable.h
    jesus.h
    jesus.c
    "list.c"
    "list.h"
    stack.c
    stack.h
    vector_testonly.h
    vector.c
    vector.h
    node.h
    node.c
    data.h
    data.c
    semantictype.h
    semantictype.c
    symbolelement.c
    symbolelement.h
    logger.h
    )

add_library(${PROJECT_NAME} STATIC ${UTILITY_FILES})

if (MSVC)
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/${PROJECT_NAME})

else()
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/${PROJECT_NAME}/${CMAKE_BUILD_TYPE})
endif()
