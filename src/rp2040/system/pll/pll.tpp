
#ifndef RP2040_SYSTEM_PLL_T_H
#define RP2040_SYSTEM_PLL_T_H

#include "include/rp2040/system/pll.h"

namespace rp2040::system {
    /**
     * @addtogroup rp2040_pll
     * @{
     */

    template <std::uintptr_t pll_addr>
    PLL_Type<pll_addr>& PLL_Type<pll_addr>::getInstance() {
        static PLL_Type<pll_addr> instance;
        return instance;
    }

    /** @} */ // rp2040_pll
} // rp2040::system


#endif // RP2040_SYSTEM_PLL_T_H