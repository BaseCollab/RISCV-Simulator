cmake_minimum_required(VERSION 3.13)

set(SOURCES main.cpp
            common/stack_test.cpp
            memory/memory_test.cpp
            mmu/mmu_test.cpp
)

add_library(unit_tests_impl OBJECT ${SOURCES})
target_include_directories(unit_tests_impl PUBLIC ${SIMULATOR_ROOT})

target_link_libraries(unit_tests PUBLIC unit_tests_impl)

add_custom_target(run_unit_tests
    COMMENT "Running unit tests"
    COMMAND ${SIMULATOR_BINARY_ROOT}/unit_tests --gtest_color=yes
)
add_dependencies(run_unit_tests unit_tests)
