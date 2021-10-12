cmake_minimum_required(VERSION 3.15)

project(Prekladac)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)

set(CMAKE_SUPPRESS_REGENERATION true)

add_subdirectory(src/LexicalAnalyzer)
add_subdirectory(tests/LexicalAnalyzer)
add_subdirectory(src/Utils)
add_subdirectory(tests/Utils)

if(CMAKE_COMPILER_IS_GNUCXX)
add_custom_target(runLexicalAnalyzerTests
    COMMAND ./build/tests/${CMAKE_BUILD_TYPE}/LexicalAnalyzerTests $(l) $(u)
)
endif()