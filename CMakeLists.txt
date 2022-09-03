cmake_minimum_required(VERSION 3.13)

project(UTFIA)

enable_testing()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)

if (NOT TARGET utee)
	set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules")
endif()

include_directories(.)

add_compile_options(
	"$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:$<$<COMPILE_LANGUAGE:CXX>:-fPIC;-fshort-wchar;-Wpedantic;-Wall;-Wextra;-Wno-long-long>>"
	"$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-O3>>"
	"$<$<CXX_COMPILER_ID:MSVC>:$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-Ox;-W4>>"
)

add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(tests/utfia)
add_subdirectory(tests/utfia.benchmark)

add_subdirectory(libraries/utee)

add_test(NAME utfia.tests COMMAND $<TARGET_FILE:utee-runner> $<TARGET_FILE:utfia.tests> WORKING_DIRECTORY $<TARGET_FILE_DIR:utfia.tests>)
add_test(NAME utfia.benchmark COMMAND $<TARGET_FILE:utfia.benchmark>)
