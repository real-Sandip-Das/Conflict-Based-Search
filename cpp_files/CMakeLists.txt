cmake_minimum_required(VERSION 3.10)

project(ConflictBasedSearch)

set(CMAKE_CXX_STANDARD 17)

# Define sources
set(SOURCES
        src/ConflictTree.cpp
        )

# Define headers
set(HEADERS
        include/ConflictTree.h
        include/definitions.h
        )

# Add executable
add_executable(ConflictBasedSearch ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(ConflictBasedSearch PUBLIC include)