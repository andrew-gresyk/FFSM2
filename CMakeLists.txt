cmake_minimum_required(VERSION 3.5)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
	message(STATUS "Found ccache in ${CCACHE_PROGRAM}")
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(ffsm2_test)

file(GLOB SOURCE_FILES "test/*.cpp" "test/shared/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/development"
	"${CMAKE_CURRENT_LIST_DIR}/external"
	"${CMAKE_CURRENT_LIST_DIR}/include")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wold-style-cast)
endif()

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME}
				   POST_BUILD
				   COMMAND ${PROJECT_NAME})
