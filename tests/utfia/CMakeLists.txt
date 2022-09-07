cmake_minimum_required(VERSION 3.13)

set(UTFIA_TEST_SOURCES
	StringTests.cpp
	UTF8IterationTests.cpp
)

add_compile_options("$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-std=c++03>")

add_library(utfia.tests SHARED ${UTFIA_TEST_SOURCES})
target_link_libraries(utfia.tests utfia)
add_utee_test(utfia.tests)
