cmake_minimum_required(VERSION 2.6)
 
# setting variables
# set(dir ${CMAKE_CURRENT_SOURCE_DIR})
# set(CMAKE_BUILD_EXE ${dir}/build/bin)
# set(CMAKE_BUILD_TESTS ${dir}/build/tests)
# set(CMAKE_SOURCE_DIRECTORY ${dir}/src)
# set(CMAKE_TEST_DIRECTORY ${dir}/tests)
# set(BUILD_PATH /build/tests CACHE PATH "...")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
# add_executable(${BUILD_PATH}/runHelloWorldTest tests/HelloWorldTest.cpp)
# target_link_libraries(${BUILD_PATH}/runHelloWorldTest ${GTEST_LIBRARIES} pthread)

add_executable(build/tests/runHelloWorldTest tests/HelloWorldTest.cpp)
target_link_libraries(build/tests/runHelloWorldTest ${GTEST_LIBRARIES} pthread)

add_custom_target(runTest
    COMMAND ./build/tests/runHelloWorldTest $(l) $(u)
)