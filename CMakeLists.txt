cmake_minimum_required(VERSION 3.15)

project(Prekladac)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)

set(CMAKE_SUPPRESS_REGENERATION true)

set(MAIN_SRC "${CMAKE_SOURCE_DIR}/src/main.c")

add_subdirectory(src/LexicalAnalyzer)
add_subdirectory(src/Utils)
add_subdirectory(src/SyntaxAnalyzer)

include_directories(${CMAKE_SOURCE_DIR}/src/SyntaxAnalyzer)
link_directories(${CMAKE_SOURCE_DIR}/build/bin/SyntaxAnalyzer/${CMAKE_BUILD_TYPE})
include_directories(${CMAKE_SOURCE_DIR}/src/LexicalAnalyzer)
link_directories(${CMAKE_SOURCE_DIR}/build/bin/LexicalAnalyzer/${CMAKE_BUILD_TYPE})
link_directories(${CMAKE_SOURCE_DIR}/src/Utils)
link_directories(${CMAKE_SOURCE_DIR}/build/bin/Utils/${CMAKE_BUILD_TYPE})

add_executable(main ${MAIN_SRC})
target_link_libraries(main SyntaxAnalyzer)
target_link_libraries(main LexicalAnalyzer)
target_link_libraries(main Utils)

add_custom_target(run
    COMMAND ./main $(l) $(u)
)

add_subdirectory(tests/LexicalAnalyzer)
add_subdirectory(tests/SyntaxAnalyzer)
add_subdirectory(tests/Utils)
add_subdirectory(LLTableGenerator)