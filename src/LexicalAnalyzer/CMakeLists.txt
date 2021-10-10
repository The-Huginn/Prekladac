cmake_minimum_required(VERSION 3.15)

project(LexicalAnalyzer)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)


set(LEXICAL_ANALYZER_FILES
    LexicalAnalyzer.c
    LexicalAnalyzer.h   
    LexicalFSM.c
    LexicalFSM.h
    )

add_library(${PROJECT_NAME} STATIC ${LEXICAL_ANALYZER_FILES})

if (MSVC)
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/${PROJECT_NAME})

else()
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/${PROJECT_NAME}/${CMAKE_BUILD_TYPE})

endif()

