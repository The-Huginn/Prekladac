cmake_minimum_required(VERSION 3.15)

project(LexicalAnalyzerTests)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)


set(LEXICAL_ANALYZER_FILES
    LexicalAnalyzerTests.cpp
    )


add_executable(${PROJECT_NAME} ${LEXICAL_ANALYZER_FILES})

# Locate GTest
if(CMAKE_COMPILER_IS_GNUCXX)
    find_package(GTest REQUIRED)
    target_include_directories(${PROJECT_NAME} ${GTEST_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} libLexicalAnalyzer.a pthread)
endif()

if (MSVC)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/googletest-master/googletest/include)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/LexicalAnalyzer)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/Utils)
    target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/googletest-master/lib/${CMAKE_BUILD_TYPE})
    target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/build/bin/LexicalAnalyzer/${CMAKE_BUILD_TYPE})
    target_link_libraries(${PROJECT_NAME} gtest.lib gtest_main.lib LexicalAnalyzer.lib)

    add_dependencies(${PROJECT_NAME} LexicalAnalyzer)

    set_target_properties( ${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/tests/${CMAKE_BUILD_TYPE})
endif()
