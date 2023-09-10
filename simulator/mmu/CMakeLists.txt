cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0079 NEW)

set(SOURCES mmu.cpp)

add_library(mmu_impl OBJECT ${SOURCES})
target_include_directories(mmu_impl PUBLIC ${SIMULATOR_ROOT}/common ${SIMULATOR_ROOT}/hart ${SIMULATOR_ROOT}/memory ${SIMULATOR_ROOT}/supervisor)

target_link_libraries(simulator PUBLIC mmu_impl)
