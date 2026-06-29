/**
 * @file pll.h
 * @brief The interface to configure the pll.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 24.06.2026
 *
 *
 * @copyright (C) 2026  Thorsten Hoffmeister
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RP2040_SYSTEM_PLL_H
#define RP2040_SYSTEM_PLL_H

#include <bitset>
#include "pll_def.h"

namespace rp2040::system {
    /**
     * @addtogroup rp2040_pll
     * @{
     */

    /**
     * @brief The pll object.
     *
     * @tparam pll_addr The address of the pll.
     */
    template <std::uintptr_t pll_addr>
    class PLL_Type {
        static_assert((pll_addr == pll_sys_base) || (pll_addr == pll_usb_base), "Invalid pll address.");

        using pll_regs = PLL_RegMapType<pll_addr>;
        using cs_r = pll_regs::cs;
        using pwr_r = pll_regs::pwr;
        using fbdiv_int_r = pll_regs::fbdiv_int;
        using prim_r = pll_regs::prim;

    public:
        static PLL_Type<pll_addr>& getInstance();

    private:
        PLL_Type() = default;

        enum class Flags : std::uint32_t {
            isInitialized = 0U,
            NumberOfFlags
        };

        std::bitset<static_cast<std::uint32_t>(Flags::NumberOfFlags)> m_flags;
    };

    using PLL_Sys = PLL_Type<pll_sys_base>;
    using PLL_USB = PLL_Type<pll_usb_base>;
    /** @} */ // rp2040_pll
} // rp2040::system


#include "../../../pll.tpp"

#endif // RP2040_SYSTEM_PLL_H
