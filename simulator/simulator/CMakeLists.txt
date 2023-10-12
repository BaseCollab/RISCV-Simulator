cmake_minimum_required(VERSION 3.13)

set(SOURCES simulator.cpp
            elf_loader.cpp
)

add_library(simulator_impl OBJECT ${SOURCES})
target_include_directories(simulator_impl PUBLIC ${SIMULATOR_ROOT})
target_link_libraries(simulator_impl -lelf)
