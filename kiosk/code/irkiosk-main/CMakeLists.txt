cmake_minimum_required(VERSION 3.2)
project(irkiosk-main-tests)

string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

include_directories(${CMAKE_SOURCE_DIR}/lily-test)
add_executable(kiosk-tests 
	${CMAKE_SOURCE_DIR}/test-main.cpp
)
set_target_properties(
	kiosk-tests PROPERTIES 
	CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -Wpedantic -pedantic-errors"
)
