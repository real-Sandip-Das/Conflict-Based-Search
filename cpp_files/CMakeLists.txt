cmake_minimum_required(VERSION 3.20)

project(ConflictBasedSearch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(SOURCES
        src/AStar.cpp
        src/ConflictTree.cpp
        src/main.cpp
        src/ConvertMap.cpp
)

set(HEADERS
        include/definitions.h
        include/AStar.h
        include/ConflictTree.h
        )

add_executable(ConflictBasedSearch ${SOURCES})
target_include_directories(ConflictBasedSearch PUBLIC include)

find_package(GTest CONFIG REQUIRED)
add_executable(cbs_tests
        tests/test_convertmap.cpp
        tests/test_benchmark.cpp
        src/ConvertMap.cpp
        src/AStar.cpp
        src/ConflictTree.cpp
)
target_include_directories(cbs_tests PUBLIC include)
target_link_libraries(cbs_tests PRIVATE GTest::gtest GTest::gtest_main)