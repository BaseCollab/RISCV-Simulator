cmake_minimum_required(VERSION 3.13)


add_library (
    Cosimulation cosimulation.cpp
)

set_property(TARGET Cosimulation PROPERTY POSITION_INDEPENDENT_CODE ON)

set(SOURCES cosimulation.cpp)

target_compile_options(Cosimulation PRIVATE -fPIC)

add_compile_options(-fPIC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")

add_library(cosim_impl OBJECT ${SOURCES})
target_include_directories(cosim_impl PUBLIC ${SIMULATOR_ROOT})

