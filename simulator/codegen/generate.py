#!/usr/bin/env python3

import yaml
import sys

COMMENT_NO_CHANGE_STR = "/*\n"                                                                      \
                 " * This file is autogenerated by 'generate.py' script from risc-v.yaml file\n"    \
                 " * Do not try to change anything in this file\n"                                  \
                 " ***********************************************************\n"                   \
                 " * Written by Glaz Roman (Vokerlee)\n"                                            \
                 "*/\n\n"

COMMENT_WITH_CHANGE_STR = "/*\n"                                                                    \
                 " * This file is autogenerated by 'generate.py' script from risc-v.yaml file\n"    \
                 " * This file can be changed without additional autogeneration\n"                  \
                 " ***********************************************************\n"                   \
                 " * Written by Glaz Roman (Vokerlee)\n"                                            \
                 "*/\n\n"

def parse_yaml(filename):
    with open(filename, 'r') as instr_file:
        return yaml.safe_load(instr_file)

def get_bits_str(msb, lsb, word_str):
    return f'bitops::GetBits<{msb}, {lsb}>(' + word_str + ')'

def sort_bits(bits):
    return sorted(bits, key=lambda elem : elem['from'])

def sign_extend_str(old_size, new_size):
    return f'bitops::SignExtend<{old_size}, {new_size}>'

def set_invalid_id():
    print('/* Instruction wasn\'t found */\n' \
          'instr->id = InstructionId::INVALID_ID;\n' \
          '\n' \
          'return iexec::INVALID(this, *instr);\n' \
          )

def write_fields_fill(decoder_leaf, fields, mode):
    instr_fields = decoder_leaf['fields']
    instr_name = decoder_leaf['mnemonic'].upper().replace('.', '_')

    print(f'instr->id = InstructionId::{instr_name};\n')

    if decoder_leaf['format'] == 'B' or decoder_leaf['mnemonic'] in ['jalr', 'jal', 'ecall']:
        print('instr->attributes.is_branch = true;\n')
    elif decoder_leaf['format'] == 'S':
        print('instr->attributes.is_store = true;\n')
    elif decoder_leaf['format'] == 'I' and instr_name[0] == 'L':
        print('instr->attributes.is_load = true;\n')

    if mode == "privileged":
        if instr_name == 'WFI' or instr_name[0] == 'M':
            print('instr->attributes.mode = Mode::MACHINE_MODE;\n')
        elif instr_name[0] == 'H':
            print('instr->attributes.mode = Mode::HYPERVISOR_MODE;\n')
        elif instr_name[0] == 'S':
            print('instr->attributes.mode = Mode::SUPERVISOR_MODE;\n')

    for field in instr_fields:
        name = fields[field]['name']
        bits_list = fields[field]['location']['bits']
        bits_list = sort_bits(bits_list)

        for elem in bits_list:
            msb, lsb = elem['msb'], elem['lsb']
            move = elem['to']

            # TEMPORARY EXCLUSION
            exclude_list = ['fm', 'pred', 'succ', 'aqrl', 'shamtw', 'shamt']
            if name in exclude_list:
                if name in exclude_list[3:]:
                    msb = 31
                name = 'imm'

            print(f'instr->{name} |= ', end='')
            if name == 'imm' and msb == 31:
                print(f'{sign_extend_str(msb - lsb + 1, "bitops::BitSizeof<word_t>()")}')

            print(f'({get_bits_str(msb, lsb, "raw_instr")})', end='')
            print(';' if move == 0 else f' << {move};')

            if (name == 'rd'):
                print(f'if (instr->{name} == 0) {{')
                print(f'instr->{name} = SINK_REG_IDX;\n', end='')
                print(f'}}\n')

    print(f'\n#ifdef DEBUG_HART\n', end='')
    print(f'std::cerr << \"[DEBUG] [DECODE] \" << std::hex << \"{instr_name}: 0x\" << raw_instr << std::dec << std::endl;\n', end='')
    print(f'#endif // DEBUG_HART\n', end='')

    print(f'\nreturn iexec::{instr_name}(this, *instr);\n', end='')

def recursive_parse(decoder_tree, fields, mode):
    if 'range' in decoder_tree:
        opcode_str = get_bits_str(decoder_tree['range']['msb'], decoder_tree['range']['lsb'], 'raw_instr')
        var_name = f'var_bits_{recursive_parse.var_cnt}'

        recursive_parse.var_cnt += 1

        print(f'word_t {var_name} = {opcode_str};')

        for node in decoder_tree['nodes']:
            if recursive_parse.var_cnt == 1 and 0b1110011 == node: # system opcode
                mode = "privileged"

            print(f'if ({var_name} == {node})\n{{')
            recursive_parse(decoder_tree['nodes'][node], fields, mode)
            print('}\n')

            if recursive_parse.var_cnt == 1:
                mode = "unprivileged"

        recursive_parse.var_cnt -= 1
        set_invalid_id()
    else:
        write_fields_fill(decoder_tree, fields, mode)

recursive_parse.var_cnt = 0

def decode_gen(fout, yaml_dict):
    stdout_bak = sys.stdout
    sys.stdout = fout

    recursive_parse(yaml_dict['decodertree'], yaml_dict['fields'], "unprivileged")

    sys.stdout = stdout_bak

def generate_decode_logic(yaml_dict):
    with open('execute.cpp', 'w') as fout:
        fout.write(COMMENT_NO_CHANGE_STR)
        fout.write('#include \"hart/hart.h\"\n')
        fout.write('#include \"hart/exception.h\"\n')
        fout.write('#include \"common/utils/bit_ops.h\"\n')
        fout.write('#include \"instruction/instruction.h\"\n')
        fout.write('#include \"instruction/instruction_exec.h\"\n\n')
        fout.write('namespace rvsim {\n\n')
        fout.write('Exception Hart::DecodeAndExecute(Instruction *instr, instr_size_t raw_instr)\n{\n')

        decode_gen(fout, yaml_dict)

        fout.write('}\n')
        fout.write("\n} // namespace rvsim\n")

def generate_execution_decls(yaml_dict):
    instrs = yaml_dict['instructions']

    instrs_mnemonic_list = []
    for instr in instrs:
        instrs_mnemonic_list.append(instr['mnemonic'].upper().replace('.', '_'))

    with open('instruction_exec.h', 'w') as fout:
        fout.write(COMMENT_NO_CHANGE_STR)
        fout.write("#ifndef SIMULATOR_INSTRUCTION_EXEC_INSTRUCTION_EXEC_H\n"
                   "#define SIMULATOR_INSTRUCTION_EXEC_INSTRUCTION_EXEC_H\n\n")
        fout.write('#include \"hart/hart.h\"\n')
        fout.write('#include \"hart/exception.h\"\n')
        fout.write('#include \"instruction.h\"\n\n')

        fout.write('namespace rvsim {\n'
                   'namespace iexec {\n\n'
                   '// clang-format off\n\n')

        max_instr_len = max([len(instr) for instr in instrs_mnemonic_list])

        for i in range(len(instrs_mnemonic_list)):
            fout.write(f'Exception {instrs_mnemonic_list[i]}' + \
                        ' ' * (max_instr_len - len(instrs_mnemonic_list[i])) + '(Hart &hart, const Instruction &instr);\n')

        fout.write('\n// clang-format on\n')
        fout.write("\n} // namespace iexec\n")
        fout.write("} // namespace rvsim\n\n")
        fout.write("#endif // SIMULATOR_INSTRUCTION_EXEC_INSTRUCTION_EXEC_H\n")

    with open('instruction_exec.cpp', 'w') as fout:
        fout.write(COMMENT_WITH_CHANGE_STR)
        fout.write('#include \"instruction_exec.h\"\n\n')

        fout.write('namespace rvsim {\n'
                   'namespace iexec {\n\n')

        max_instr_len = max([len(instr) for instr in instrs_mnemonic_list])

        for i in range(len(instrs_mnemonic_list)):
            fout.write(f'Exception {instrs_mnemonic_list[i]}(Hart &hart, const Instruction &instr)\n'
                        '{\n')
            fout.write('    (void) hart;\n')
            fout.write('    (void) instr;\n')
            fout.write(f'    std::cerr << \"function iexec::{instrs_mnemonic_list[i]}(Hart &hart, const Instruction &instr) '
                       'is not implemented yet!\" << std::endl;\n')
            fout.write("}\n\n")

        fout.write("} // namespace iexec\n")
        fout.write("\n} // namespace rvsim\n")

def generate_id_enum(yaml_dict):
    instrs = yaml_dict['instructions']

    instrs_mnemonic_list = []
    for instr in instrs:
        instrs_mnemonic_list.append(instr['mnemonic'].upper().replace('.', '_'))

    instrs_mnemonic_list.append('PSEUDO') # pseudo cmd for plugin implementation

    with open('instruction_id.h', 'w') as fout:
        fout.write(COMMENT_NO_CHANGE_STR)
        fout.write("#ifndef SIMULATOR_INSTRUCTION_ID_INSTRUCTION_ID_H\n"
                   "#define SIMULATOR_INSTRUCTION_ID_INSTRUCTION_ID_H\n\n"
                   "namespace rvsim {\n\n"
                   "// clang-format off\n\n")

        fout.write('enum class InstructionId\n'
                   '{\n')

        max_instr_len = max([len(instr) for instr in instrs_mnemonic_list])

        fout.write('    INVALID_ID' + ' ' * (max_instr_len - len('INVALID_ID')) + ' = -1,\n')

        for i in range(len(instrs_mnemonic_list)):
            fout.write(f'    {instrs_mnemonic_list[i]}' + ' ' * (max_instr_len - len(instrs_mnemonic_list[i])) + \
                       ' = ' + str(i + 1) + ',\n')

        fout.write("};\n\n"
                   "// clang-format on\n\n"
                   "} // namespace rvsim\n\n")

        fout.write("#endif // SIMULATOR_INSTRUCTION_ID_INSTRUCTION_ID_H\n")

def main():
    if len(sys.argv) != 2:
        print('Invalid amount of arguments [1 must be used]',        file=sys.stderr)
        print('Usage example: ' + sys.argv[0] + ' ' + 'risc-v.yaml', file=sys.stderr)

    yaml_dict = parse_yaml(sys.argv[1])

    generate_id_enum(yaml_dict)
    generate_decode_logic(yaml_dict)
    generate_execution_decls(yaml_dict)

if __name__ == '__main__':
    main()
