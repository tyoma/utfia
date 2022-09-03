cmake_minimum_required(VERSION 3.13)

project(utfia)

enable_testing()

option(UTFIA_NO_TESTS "Do not build test modules." OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)

add_compile_options(
	"$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:$<$<COMPILE_LANGUAGE:CXX>:-fPIC;-fshort-wchar;-Wpedantic;-Wall;-Wextra;-Wno-long-long>>"
	"$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-O3>>"
	"$<$<CXX_COMPILER_ID:MSVC>:$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-Ox;-W4>>"
)
add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)

add_library(utfia INTERFACE)
target_include_directories(utfia INTERFACE .)

if (NOT UTFIA_NO_TESTS)
	if (NOT TARGET utee)
		set(UTEE_NO_TESTS ON)
		set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/libraries/utee/make/build.props)
		add_subdirectory(libraries/utee)
	endif()
	add_subdirectory(tests/utfia)
	add_subdirectory(tests/utfia.benchmark)
endif()
