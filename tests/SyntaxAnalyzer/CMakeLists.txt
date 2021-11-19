cmake_minimum_required(VERSION 3.15)

project(SyntaxAnalyzerTests)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)


set(SYNTAX_ANALYZER_FILES
    SyntaxAnalyzerTests.cpp
    )

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/bin/tests")


add_executable(${PROJECT_NAME} ${SYNTAX_ANALYZER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/SyntaxAnalyzer)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/LexicalAnalyzer)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/Utils)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/build/bin/SyntaxAnalyzer/${CMAKE_BUILD_TYPE})
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/build/bin/LexicalAnalyzer/${CMAKE_BUILD_TYPE})
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/build/bin/Utils/${CMAKE_BUILD_TYPE})

# Locate GTest
if(CMAKE_COMPILER_IS_GNUCXX)
    find_package(GTest REQUIRED)
    target_include_directories(${PROJECT_NAME} PUBLIC ${GTEST_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} libSyntaxAnalyzer.a libUtils.a libLexicalAnalyzer.a pthread)

    set_target_properties( ${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE})
endif()

if (MSVC)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/googletest-master/googletest/include)
    target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/googletest-master/lib)
    target_link_libraries(${PROJECT_NAME} gtest.lib gtest_main.lib SyntaxAnalyzer.lib LexicalAnalyzer.lib Utils.lib)

    add_dependencies(${PROJECT_NAME} SyntaxAnalyzer LexicalAnalyzer Utils)

    set_target_properties( ${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_DIR}/$(Configuration)
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
