cmake_minimum_required(VERSION 3.20)

project(ConflictBasedSearch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
        src/AStar.cpp
        src/main.cpp
        )

set(HEADERS
        include/definitions.h
        include/AStar.h
        )

add_executable(ConflictBasedSearch ${SOURCES})

target_include_directories(ConflictBasedSearch PUBLIC include)