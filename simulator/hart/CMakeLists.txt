cmake_minimum_required(VERSION 3.13)

set(SOURCES hart.cpp
            exception_handler.cpp
)

add_library(hart_impl OBJECT ${SOURCES})
target_include_directories(hart_impl PUBLIC ${SIMULATOR_ROOT})
