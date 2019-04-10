cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Werror")

project(ffsm_test)

include_directories("${CMAKE_CURRENT_LIST_DIR}/external"
					"${CMAKE_CURRENT_LIST_DIR}/include")

file(GLOB SOURCE_FILES "test/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_test(NAME ffsm_test COMMAND ffsm_test)

add_custom_command(TARGET ffsm_test
				   POST_BUILD
				   COMMAND ffsm_test)
