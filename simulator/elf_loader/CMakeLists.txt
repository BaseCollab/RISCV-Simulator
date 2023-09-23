cmake_minimum_required(VERSION 3.13)

set(SOURCES elf_loader.cpp)

add_library(elf_loader_impl OBJECT ${SOURCES})
target_include_directories(elf_loader_impl PUBLIC ${SIMULATOR_ROOT})

target_link_libraries(elf_loader_impl -lelf)
