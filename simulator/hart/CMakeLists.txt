cmake_minimum_required(VERSION 3.13)

set(SOURCES hart.cpp
            basic_block_manager.cpp
            exception_handler.cpp
)

add_library(hart_impl OBJECT ${SOURCES})
target_include_directories(hart_impl PUBLIC 
                           ${SIMULATOR_ROOT}
                           ${SIMULATOR_BINARY_ROOT}
)

add_dependencies(hart_impl decoder_impl)
target_link_libraries(hart_impl decoder_impl)
