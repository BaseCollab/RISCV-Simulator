cmake_minimum_required(VERSION 3.13)

set(SOURCES parser.cpp)

add_library(parser_impl OBJECT ${SOURCES})
target_include_directories(parser_impl PUBLIC ${SIMULATOR_ROOT})
