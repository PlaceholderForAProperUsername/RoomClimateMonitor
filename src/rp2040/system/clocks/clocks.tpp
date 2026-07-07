#ifndef RP2040_SYSTEM_CLOCKS_T_H
#define RP2040_SYSTEM_CLOCKS_T_H
#include "rp2040/system/clocks.h"

namespace rp2040::system::clocks {

    template <std::uintptr_t ref_clk_addr>
    Ref_Type& Ref_Type<ref_clk_addr>::getInstance() {
        static Ref_Type<ref_clk_addr> instance;
        return instance;
    }
}

#endif // RP2040_SYSTEM_CLOCKS_T_H