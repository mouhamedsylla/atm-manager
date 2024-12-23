cmake_minimum_required(VERSION 3.10)

project(atm C)


find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(atm ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} 
	PRIVATE
	     ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} 
	PRIVATE
	     ${CURSES_LIBRARIES}
)
