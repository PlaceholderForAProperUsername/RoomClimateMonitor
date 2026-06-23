/**
 * @file pll_def.h
 * @brief Definitions for the phase-locked loop (PLL).
 * @author Thorsten Hoffmeister
 * @version 1.0
 * @date 19.06.2026
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

#ifndef RP2040_SYSTEM_PLL_DEF_H
#define RP2040_SYSTEM_PLL_DEF_H

#include <cstdint>

#include "utils/reg_access.h"

namespace rp2040::system {
    /**
     * @addtogroup rp2040_pll
     * @{
     */

    constexpr std::uintptr_t pll_sys_base = 0x40028000U; /**< @brief The address of the system pll. */
    constexpr std::uintptr_t pll_usb_base = 0x4002c000U; /**< @brief The address of the usb pll. */

    /**
     * @brief The register map of the pll
     *
     * @tparam pll_addr The address of the pll.
     */
    template <std::uintptr_t pll_addr>
    struct PLL_RegMapType {
        static_assert((pll_addr == pll_sys_base) || (pll_addr == pll_usb_base), "Invalid pll address.");

        static constexpr std::uint32_t cs_offset = 0x00U; /**< @brief The offset of the control and status register. */
        static constexpr std::uint32_t pwr_offset = 0x04U; /**< @brief The offset of the power register. */
        static constexpr std::uint32_t fbdiv_int_offset = 0x08U; /**< @brief The offset of the feedback divisor register. */
        static constexpr std::uint32_t prim_offset = 0x0CU; /**< @brief The offset of the primary output register to control the post dividers. */

        /**
         * @brief The control and status register.
         */
        struct cs {
            static constexpr std::uintptr_t addr = pll_addr + cs_offset; /**< @brief The address of the cs register. */
            using cs_reg = utils::reg_access::Reg<addr, utils::reg_access::reg_mixed_access, std::uint32_t>; /**< @brief The cs register */

            /**
             * @brief The lock bits of the cs register as a bitfield.
             */
            struct LockBitField {
                using reg = cs_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static constexpr T position = 31U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the bits. */

                /**
                 * @brief Values to check if the pll is stable.
                 */
                enum class value : T {
                    unstable = 0x0U, /**< @brief The output frequency is unstable. */
                    locked = 0x01U, /**< @brief The output frequency is stable. */
                };
            };
            using lock_bits = cs_reg::template Bits<LockBitField, utils::reg_access::read_access>; /**< @brief The lock bit of the cs register. */


        };
    };

    /** @}*/ // rp2040_pll
}

#endif // RP2040_SYSTEM_PLL_DEF_H
