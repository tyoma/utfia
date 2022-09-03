cmake_minimum_required(VERSION 3.13)

add_executable(utfia.benchmark main.cpp)
target_link_libraries(utfia.benchmark utfia)

add_test(NAME utfia.benchmark COMMAND $<TARGET_FILE:utfia.benchmark>)
