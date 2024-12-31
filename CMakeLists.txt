cmake_minimum_required(VERSION 3.10)
project(atm C)

# Required packages
find_package(Curses REQUIRED)
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(${CURSES_INCLUDE_DIR})
include_directories(${SQLITE3_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Sources
file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

# Executable
add_executable(atm ${SOURCES} ${HEADERS})

# Include directories configuration
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CURSES_INCLUDE_DIRS}
        ${SQLITE3_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${CURSES_LIBRARIES}
        SQLite::SQLite3
        form
)