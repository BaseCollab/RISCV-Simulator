cmake_minimum_required(VERSION 3.13)

set(SOURCES supervisor.cpp
            elf_loader.cpp
            exception_handling.cpp
)

add_library(supervisor_impl OBJECT ${SOURCES})
target_include_directories(supervisor_impl PUBLIC ${SIMULATOR_ROOT})
target_link_libraries(supervisor_impl -lelf)
