cmake_minimum_required(VERSION 3.0)
project(Learning)
set(SOURCE Learning_2.cpp)

add_executable(prog ${SOURCE})
target_link_libraries(prog pthread  ${CMAKE_SOURCE_DIR}/library/libTimerLib.a)

