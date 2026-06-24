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
             *
             * The lock bit indicates if the pll is locked, meaning the output frequency is stable or not.
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

            /**
             * @brief The bypass bits of the cs register.
             *
             * If the bypass is enabled, the reference clock will be passed directly to the output instead of the divided
             * voltage controlled oscillator (VCO):
             */
            using bypass_bits = cs_reg::template Bits<utils::reg_access::BitFieldEnableDisable<cs_reg, 0x08U>>;

            /**
             * @brief The bits to set the divisor of the reference clock as a bitfield.
             *
             * The behavior is undefined for a value of 0.
             *
             * @tparam Value The value to be set to the refdiv bits.
             */
            template <std::uint32_t Value>
            struct RefDivBitField {
                using reg = cs_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static constexpr T position = 0U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x3FU << position); /**< @brief The mask of the bits. */

                static_assert((1 <= Value) && (Value <= 0x3FU), "Invalid ref divider.");

                /**
                 * @brief The value to be set to the refdiv bits.
                 *
                 * Note: This enum is necessary to have a unified interface for the bitfields.
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The refdiv bits of the cs register.
             *
             * @tparam Value The value to be set to the refdiv bits.
             */
            template <std::uint32_t Value>
            using refdiv_bits = cs_reg::template Bits<RefDivBitField<Value>>;
        };


        /**
         * @brief The pwr register controls the power to parts of the pll.
         */
        struct pwr {
            static constexpr std::uintptr_t addr = pll_addr + pwr_offset; /**< @brief The address of the pwr register. */
            using pwr_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The pwr register */


            /**
             * @brief The bits to power down the vco.
             *
             * If this bit is set to enabled, the vco is powered down. Disabling this bit end the power down mode.
             */
            using vcopd_bits = pwr_reg::template Bits<utils::reg_access::BitFieldEnableDisable<pwr_reg, 0x05U>>;

            /**
             * @brief The bits to power down the post dividers.
             *
             * If this bit is set to enabled, the post dividers are powered down. Disabling this bit powers the post
             * dividers.
             */
            using postdivpd_bits = pwr_reg::template Bits<utils::reg_access::BitFieldEnableDisable<pwr_reg, 0x03U>>;

            /**
             * @brief The bit to power down the PLL.
             *
             * If this bit is set to enabled, the pll is powered down. Disabling this bit enables power to the PLL.
             */
            using pd_bits = pwr_reg::template Bits<utils::reg_access::BitFieldEnableDisable<pwr_reg, 0x00U>>;
        };

        /**
         * @brief The fbdiv_int register stores the feedback divider as an integer.
         */
        struct fbdiv_int {
            static constexpr std::uintptr_t addr = pll_addr + fbdiv_int_offset; /**< @brief The address of the fbdiv_int register. */
            using fbdiv_int_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The fbdiv_int register. */


            /**
             * @brief The feedback divisor bits as a bitfield.
             *
             * @tparam Value The value to which the feedback divisor is to be set.
             */
            template <std::uint32_t Value>
            struct FBDivBitField {
                using reg = fbdiv_int_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static constexpr T position = 0U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0xFFFU << position); /**< @brief The mask of the bits. */

                static_assert((16 <= Value) && (Value <= 320), "Invalid feedback divider value. Allowed values 16 <= value <= 320");

                /**
                 * @brief The value to which the fbdiv_int register is to be set.
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The feedback divisor bits.
             *
             * @tparam Value The value to which the feedback divisor is to be set.
             */
            template <std::uint32_t Value>
            using fbdiv_int_bits = fbdiv_int_reg::template Bits<FBDivBitField<Value>>;
        };

        /**
         * @brief The prim register to set the post dividers.
         */
        struct prim {
            static constexpr std::uintptr_t addr = pll_addr + prim_offset; /**< @brief The address of the prim register. */
            using prim_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The prim register. */


            /**
             * @brief The post divider bits as a bitfield.
             *
             * @tparam Value The value to which the postdiv bits are to be set.
             * @tparam pos The position of the postdiv bits in the register.
             */
            template <std::uint32_t Value, std::uint32_t pos>
            struct PostDivBitField {
                using reg = prim_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static_assert((pos == 12U) || (pos == 16U), "Invalid value for pos parameter. Value must be either 12 or 16!");

                static constexpr T position = pos; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x07U << position); /**< @brief The mask of the bits. */

                static_assert((1U <= Value) && (Value <= 7U), "Invalid post divider value. Value must be between 1 and 7!");

                /**
                 * @brief The value to which the postdiv bits are to be set.
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The postdiv1 bits.
             *
             * @tparam Value The value to which post divider 1 is to be set.
             */
            template <std::uint32_t Value>
            using postdiv1_bits = prim_reg::template Bits<PostDivBitField<Value, 16U>>;

            /**
             * @brief The postdiv2 bits.
             *
             * @tparam Value The value to which post divider 2 is to be set.
             */
            template <std::uint32_t Value>
            using postdiv2_bits = prim_reg::template Bits<PostDivBitField<Value, 12U>>;
        };
    };

    /** @}*/ // rp2040_pll
}

#endif // RP2040_SYSTEM_PLL_DEF_H
