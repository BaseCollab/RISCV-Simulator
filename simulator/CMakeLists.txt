cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(RISCV-SIMULATOR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SIMULATOR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SIMULATOR_BINARY_ROOT ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wshadow")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -DDEBUG_MMU -DDEBUG_ELF -DDEBUG_EXCEPTION -DDEBUG_HART")

add_subdirectory(memory)
add_subdirectory(simulator)
add_subdirectory(hart)
add_subdirectory(mmu)
add_subdirectory(argparser)
add_subdirectory(common)
add_subdirectory(codegen)
add_subdirectory(plugin)
add_subdirectory(cosimulation)

add_library(simulator_static STATIC)
target_link_libraries(simulator_static PUBLIC
    memory_impl
    hart_impl
    mmu_impl
    simulator_impl
    parser_impl
    common_impl
    plugin_impl dl
    cosim_impl
)

# --------------------------clang-format--------------------------------------

file(GLOB_RECURSE ALL_SOURCES_FILES *.cpp *.h)

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)

add_custom_target(clang-force-format
    COMMAND ${CLANG_FORMAT_EXECUTABLE}
    -i
    -style=file
    --verbose
    ${ALL_SOURCES_FILES}
)

# --------------------------google-tests--------------------------------------

enable_testing()
find_package(GTest REQUIRED)

add_executable(unit_tests)

target_link_libraries(unit_tests PUBLIC simulator_static GTest::gtest_main)

add_subdirectory(tests)

# ----------------------------------------------------------------------------

add_executable(rvsim rvsim.cpp)
target_link_libraries(rvsim PUBLIC simulator_static)

