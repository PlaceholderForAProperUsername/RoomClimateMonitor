
#ifndef RP2040_SYSTEM_PLL_T_H
#define RP2040_SYSTEM_PLL_T_H

#include "include/rp2040/system/pll.h"
#include "rp2040/system/resets.h"

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

    template <std::uintptr_t pll_addr>
    void PLL_Type<pll_addr>::init() {
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::isInitialized))) {
            return;
        }
        if constexpr (pll_addr == pll_sys_base) {
            Resets::getInstance().enable<SubsystemBits::PLL_SYS>();
        } else if constexpr (pll_addr == pll_usb_base) {
            Resets::getInstance().enable<SubsystemBits::PLL_USB>();
        }
        m_flags.set(static_cast<std::uint32_t>(Flags::isInitialized));
    }

    template <std::uintptr_t pll_addr>
    void PLL_Type<pll_addr>::deinit() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::isInitialized))) {
            return;
        }
        if constexpr (pll_addr == pll_sys_base) {
            Resets::getInstance().disable<SubsystemBits::PLL_SYS>();
        } else if constexpr (pll_addr == pll_usb_base) {
            Resets::getInstance().disable<SubsystemBits::PLL_USB>();
        }
        m_flags.reset(static_cast<std::uint32_t>(Flags::isInitialized));
    }

    /** @} */ // rp2040_pll
} // rp2040::system


#endif // RP2040_SYSTEM_PLL_T_H