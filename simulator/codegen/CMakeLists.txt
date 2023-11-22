find_package(Python3 REQUIRED)
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format-14)

set(DECODER_PATH ${SIMULATOR_ROOT}/hart/decoder.cpp)
set(INSTR_DEF_PATH ${CMAKE_CURRENT_BINARY_DIR}/generated/instruction.def)
set(INSTR_ID_PATH ${SIMULATOR_ROOT}/hart/instruction/instruction_id.h)

set(FILES_TO_GEN
    ${DECODER_PATH}
    ${INSTR_DEF_PATH}
    ${INSTR_ID_PATH}
)

add_custom_command(OUTPUT ${FILES_TO_GEN}
    COMMAND ${CMAKE_COMMAND} -E make_directory generated 
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate.py ${CMAKE_CURRENT_SOURCE_DIR}/risc-v.yaml
                                  ${SIMULATOR_ROOT}/hart/ "decoder"
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate.py ${CMAKE_CURRENT_SOURCE_DIR}/risc-v.yaml
                                  ${SIMULATOR_ROOT}/hart/instruction "iid"                         
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate.py ${CMAKE_CURRENT_SOURCE_DIR}/risc-v.yaml
                                  ${CMAKE_CURRENT_BINARY_DIR}/generated "idef"

    COMMAND ${CLANG_FORMAT_EXECUTABLE} -i -style=file --verbose ${FILES_TO_GEN}
)

add_custom_target(codegen DEPENDS ${FILES_TO_GEN})

add_library(decoder_impl ${DECODER_PATH})
add_dependencies(decoder_impl codegen)
target_include_directories(decoder_impl PUBLIC ${SIMULATOR_ROOT})
