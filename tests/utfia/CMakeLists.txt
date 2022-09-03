cmake_minimum_required(VERSION 3.13)

include(test)

set(UTFIA_TEST_SOURCES
	UTF8IterationTests.cpp
)

add_compile_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-std=c++03>")

add_library(utfia.tests SHARED ${UTFIA_TEST_SOURCES})
