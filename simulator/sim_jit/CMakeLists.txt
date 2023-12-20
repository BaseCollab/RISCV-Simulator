cmake_minimum_required(VERSION 3.13)

set(ASMJIT_EMBED TRUE)

add_definitions(-DASMJIT_STATIC)

# TODO: ask GPT how to integrate with third-party
set(ASMJIT_DIR ${SIMULATOR_ROOT}/asmjit)
include("${ASMJIT_DIR}/CMakeLists.txt")

set(SOURCES
    sim_generator.cpp
    sim_compiler.cpp
)

add_library(asmjit_compiler ${ASMJIT_SRC})

add_library(sim_jit_impl OBJECT ${SOURCES})

target_include_directories(sim_jit_impl PUBLIC ${SIMULATOR_ROOT})

target_link_libraries(sim_jit_impl 
    PRIVATE asmjit_compiler
    PRIVATE ${ASMJIT_DEPS}
)

