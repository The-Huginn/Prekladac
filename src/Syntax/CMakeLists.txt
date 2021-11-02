cmake_minimum_required(VERSION 3.15)

project(SyntaxAnalyzer)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)

set(SYNTAX_FILES
    SyntaxAnalyzer.c
    SyntaxAnalyzer.h
    LLTable.c
    LLTable.h
    Terminal.h
    NonTerminal.h
    Arrays.c
    )


add_library(${PROJECT_NAME} STATIC ${SYNTAX_FILES})

if (MSVC)
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/${PROJECT_NAME})

else()
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/${PROJECT_NAME}/${CMAKE_BUILD_TYPE})
endif()