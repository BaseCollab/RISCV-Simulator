cmake_minimum_required(VERSION 3.13)

set(SOURCES hart.cpp)

add_library(hart_impl OBJECT ${SOURCES})
target_include_directories(hart_impl PUBLIC ${SIMULATOR_ROOT}/common ${SIMULATOR_ROOT}/mmu)

target_link_libraries(simulator PUBLIC hart_impl)
