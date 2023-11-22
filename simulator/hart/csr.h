#ifndef SIMULATOR_CSR_CSR_H
#define SIMULATOR_CSR_CSR_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"

#include <cstdint>

// clang-format off
namespace rvsim {

// Control and Status Register
class CSRs {
public:
    #define DEFINE_CSR(name_caps, name, index, ...) \
        name_caps = index,

    enum Index {
        #include "csr.def"
    };

    #undef DEFINE_CSR

    #define DEFINE_CSR(name_caps, name, index, struct_fields)                                      \
        union name ## _t {                                                                         \
            struct {                                                                               \
                struct_fields                                                                      \
            } fields;                                                                              \
                                                                                                   \
            csr_t value;                                                                           \
                                                                                                   \
            name ## _t(): value(0) {}                                                              \
            name ## _t(csr_t name): value(name) {}                                                 \
            name ## _t(const name ## _t &name) { value = name.value; }                             \
                                                                                                   \
            csr_t operator=(const csr_t &name) { value = name; return name; }                      \
            name ## _t &operator=(const name ## _t &name) { value = name.value; return *this; }    \
            operator csr_t() const { return value; }                                               \
        };

    #include "csr.def"

    #undef DEFINE_CSR

public:
    NO_COPY_SEMANTIC(CSRs);
    NO_MOVE_SEMANTIC(CSRs);

    CSRs() = default;
    ~CSRs() = default;

    template <typename CSRType>
    CSRType LoadCSR(csr_idx_t index) const
    {
        return static_cast<CSRType>(csr_table_[index]);
    }

    template <typename CSRType>
    void StoreCSR(csr_idx_t index, CSRType reg)
    {
        csr_table_[index] = static_cast<csr_t>(reg);
    }

private:
    csr_t csr_table_[N_CSR]= {0};
};

#define DEFINE_CSR(name_caps, name, ...) \
    static_assert(sizeof(CSRs::name ## _t) == sizeof(csr_t), "sizeof(CSRs::" #name "_t) != sizeof(csr_t)");

#include "csr.def"

#undef DEFINE_CSR

} // namespace rvsim
// clang-format on

#endif // SIMULATOR_CSR_CSR_H
