#include "hart.h"

namespace rvsim {

csr_t Hart::loadCSR(csr_idx_t index) const
{
    return csr_table_[index];
};

void Hart::storeCSR(csr_idx_t index, csr_t reg)
{
    csr_table_[index] = reg;
};

} // rvsim
