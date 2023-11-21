#include "hart/hart.h"
#include "hart/basic_block.h"
#include "hart/instruction/instruction_exec_inl.h"
#include "plugin/plugin_handler.h"

#include <iostream>

namespace rvsim {

Exception Hart::LoadFromMemory(void *dst, size_t dst_size, vaddr_t src, uint8_t rwx_flags) const
{
    addr_t vpage_padding = (VPAGE_SIZE - src % VPAGE_SIZE) % VPAGE_SIZE;

    paddr_t paddr {0};
    Exception exception {Exception::NONE};

    if (vpage_padding != 0) {
        std::tie(paddr, exception) = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_, mode_);
        if (exception != Exception::NONE) {
            return exception;
        }

        memory_->Load(dst, vpage_padding, paddr);
    }

    for (addr_t vpage_offset = 0; vpage_offset < dst_size - vpage_padding; vpage_offset += VPAGE_SIZE) {
        std::tie(paddr, exception) =
            mmu_.VirtToPhysAddr(src + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_, mode_);
        if (exception != Exception::NONE) {
            return exception;
        }

        size_t load_size = VPAGE_SIZE;
        if ((dst_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            load_size = dst_size - vpage_padding - vpage_offset;

        memory_->Load((char *)dst + vpage_padding + vpage_offset, load_size, paddr);
    }

    return Exception::NONE;
}

Exception Hart::StoreToMemory(vaddr_t dst, void *src, size_t src_size, uint8_t rwx_flags) const
{
    addr_t vpage_padding = (VPAGE_SIZE - dst % VPAGE_SIZE) % VPAGE_SIZE;

    paddr_t paddr {0};
    Exception exception {Exception::NONE};

    if (vpage_padding != 0) {
        std::tie(paddr, exception) = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_, mode_);
        if (exception != Exception::NONE) {
            return exception;
        }

        memory_->Store(paddr, src, vpage_padding);
    }

    for (addr_t vpage_offset = 0; vpage_offset < src_size - vpage_padding; vpage_offset += VPAGE_SIZE) {
        std::tie(paddr, exception) =
            mmu_.VirtToPhysAddr(dst + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_, mode_);
        if (exception != Exception::NONE) {
            return exception;
        }

        size_t store_size = VPAGE_SIZE;
        if ((src_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            store_size = src_size - vpage_padding - vpage_offset;

        memory_->Store(paddr, (char *)src + vpage_padding + vpage_offset, store_size);
    }

    return Exception::NONE;
}

Exception Hart::FetchInstruction(instr_size_t *raw_instr) const
{
#ifdef DEBUG_HART
#ifdef DEBUG
    std::cerr << "[DEBUG] ============================================="
                 "============================================="
              << std::endl;
#endif
    std::cerr << "[DEBUG] [FETCH] PC = 0x" << std::hex << pc_ << std::dec << std::endl;
#endif

    return LoadFromMemory<instr_size_t>(vaddr_t(pc_), raw_instr, PF_R | PF_X);
}

void Hart::Interpret()
{
    is_idle_ = false;

    while (true) {
        BasicBlock *bb = bb_manager_->GetNextBB();
        ExecuteBasicBlock(bb);

        if (pc_ == 0 || is_idle_) {
            break;
        }
    }
}

// Disable warning because the function uses computed goto
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-label-as-value"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

// clang-format off
void Hart::ExecuteBasicBlock(BasicBlock *bb)
{   
    #define DEFINE_INSTR(instr) &&instr,
    #define DEFINE_BRANCH_INSTR(instr) &&instr,

    static void *dispatch_table[] = {
        0, // there is no zero element in instruction_id.h
        #include "codegen/generated/instruction.def"
    };

    #undef DEFINE_INSTR
    #undef DEFINE_BRANCH_INSTR

    const auto &instructions = bb->GetInstructions();
    size_t instr_idx = 0;
    Exception exception = Exception::NONE;

    for (auto it: instructions) {
        std::cout << "[" << static_cast<int>(it.id) << std::endl;
    }

    #define DISPATCH() goto *dispatch_table[static_cast<byte_t>(instructions[++instr_idx].id)];

    // dispatching entrypoint
    goto *dispatch_table[static_cast<byte_t>(instructions[instr_idx].id)];

    #define DEFINE_INSTR(instr)                                  \
    instr:                                                       \
        std::cout << #instr << " " << static_cast<int>(instructions[instr_idx].id) << " " << instr_idx << std::endl; \
        DumpRegs(std::cerr); \
        exception = iexec::instr(this, instructions[instr_idx]); \
        DumpRegs(std::cerr); \
        if (UNLIKELY(exception != Exception::NONE)) {            \
            handlers_.mmu_handler(exception, vaddr_t(instructions[instr_idx].pc));      \
        }                                                        \
        pc_ = pc_target_; \
        DISPATCH();

    // All basic blocks ends with branch instruction
    #define DEFINE_BRANCH_INSTR(instr)                           \
    instr:                    \
        std::cout << "branch instr:" << #instr << " " << static_cast<int>(instructions[instr_idx].id) << std::endl; \
        DumpRegs(std::cerr);                                    \
        exception = iexec::instr(this, instructions[instr_idx]); \
        DumpRegs(std::cerr); \
        if (UNLIKELY(exception != Exception::NONE)) {            \
            handlers_.mmu_handler(exception, vaddr_t(instructions[instr_idx].pc));      \
        }                                                        \
        pc_ = pc_target_;\
        goto BB_END;

    #include "codegen/generated/instruction.def"

    #undef DEFINE_INSTR
    #undef DEFINE_BRANCH_INSTR

BB_END:
    return;
}
// clang-format on

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

void Hart::InterpretWithPlugins()
{
    // start interpreting instructions
    is_idle_ = false;

    while (!is_idle_) {
        instr_size_t raw_instr = FetchInstruction();

        Instruction instr;
        DecodeAndExecute(&instr, raw_instr);
        if (instr.id == InstructionId::INVALID_ID) {
            std::cerr << "[x] Found Instruction with invalid ID\n[x] Exit.\n";
            exit(EXIT_FAILURE);
        }
        plugin_handler_->SetCurInstr(0, &instr);
        plugin_handler_->CallPlugin(0, rvsim::PluginRegimes::CoSimulation_run);

        pc_ = pc_target_;
    }
}
} // namespace rvsim
