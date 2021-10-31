cmake_minimum_required(VERSION 3.15)

project(LLTableGenerator)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_C_STANDARD 99)


set(LL_TABLE_GENERATOR_FILES
    generator.cpp
    symbol.cpp
    symbol.h
    terminal.cpp
    terminal.h
    nonterminal.cpp
    nonterminal.h
    rule.cpp
    rule.h
    )

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/bin/${PROJECT_NAME}")

add_executable(${PROJECT_NAME} ${LL_TABLE_GENERATOR_FILES})

if (MSVC)
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_DIR}/$(Configuration)
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

else()
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/${PROJECT_NAME}/${CMAKE_BUILD_TYPE})
endif()
