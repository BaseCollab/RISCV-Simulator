cmake_minimum_required(VERSION 3.13)


add_library (
    Cosim cosim.cpp
)

target_include_directories(Cosim PUBLIC ${SIMULATOR_ROOT})

set_property(TARGET Cosim PROPERTY POSITION_INDEPENDENT_CODE ON)

set(SOURCES cosim.cpp)

target_compile_options(Cosim PRIVATE -fPIC)

add_compile_options(-fPIC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")

add_library(cosim_impl OBJECT ${SOURCES})
target_include_directories(cosim_impl PUBLIC ${SIMULATOR_ROOT})
