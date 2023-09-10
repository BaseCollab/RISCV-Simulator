cmake_minimum_required(VERSION 3.13)

set(SOURCES mmu.cpp)

add_library(mmu_impl OBJECT ${SOURCES})
target_include_directories(supervisor_impl PUBLIC ${SIMULATOR_ROOT})

target_link_libraries(simulator PUBLIC mmu_impl)
