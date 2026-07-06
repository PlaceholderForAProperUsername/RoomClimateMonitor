/**
 * @file clocks_def.h
 * @brief Definitions of the clock control module.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 03.07.2026
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

#ifndef RP2040_SYSTEM_CLOCKS_DEF_H
#define RP2040_SYSTEM_CLOCKS_DEF_H

#include <cstdint>

#include "utils/reg_access.h"

namespace rp2040::system::clocks {
    /**
     * @addtogroup rp2040_clocks
     * @{
     */
    constexpr std::uintptr_t clocks_base = 0x40008000U; /**< @brief Base address of clocks. */

    constexpr std::uint32_t sys_offset = 0x3CU; /**< @brief Offset for system clock control registers. */
    constexpr std::uint32_t peri_offset = 0x48U; /**< @brief Offset for peripheral clock control registers. */

    /**
     * @brief The possible clock sources for the reference clock.
     */
    enum class RefClockSrc : std::uint32_t {
        ROSC = 0x00U, /**< @brief The ring oscillator. */
        XOSC = 0x02U, /**< @brief The crystal oscillator */
        AUX_SRC_START = 0x03, /**< @brief Helper entry. Not an actual source. */
        PLL_USB = 0x03U, /**< @brief The usb pll. */
        GPIN0 = 0x04U, /**< @brief External clock provided through GPIN0. */
        GPIN1 = 0x05U, /**< @brief External clock provided through GPIN1. */
        RefClockCount /**< @brief Helper entry for parameter checking. */
    };

    /**
     * @brief Registers for the reference clock control registers.
     */
    struct Ref_RegMapType {
        static constexpr std::uint32_t ref_base_offset = 0x30U; /**< @brief Offset for reference clock control registers. */
        static constexpr std::uint32_t ctrl_offset = 0x00U; /**< @brief Offset for the control register. */
        static constexpr std::uint32_t div_offset = 0x04U; /**< @brief Offset for the divisor register. */
        static constexpr std::uint32_t selected_offset = 0x08U; /**< @brief Offset for the selected register. */

        /**
         * @brief Register to control the reference clock.
         */
        struct ctrl {
            static constexpr std::uintptr_t addr = clocks_base + ref_base_offset + ctrl_offset; /**< @brief The address of the ctrl register of the reference clock control. */
            using ctrl_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The control register of the reference clock. */

            /**
             * @brief The auxiliary clock source bits as a BitField.
             *
             * @tparam Value The value for the auxiliary source for the reference clock.
             */
            template <std::uint32_t Value>
            struct AuxSrcBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static constexpr T position = 0x05U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0x02U, "Invalid value for the auxiliary src of the reference clock.");

                /**
                 * @brief The values to which the auxiliary clock source can be set.
                 *
                 * Values:
                 * - 0x00 PLL_USB
                 * - 0x01 GPIN0
                 * - 0x02 GPIN1
                 */
                enum class value : T {
                    val = Value
                };
            };

            /**
             * @brief The auxiliary source bits.
             *
             * @tparam Value The value for the auxiliary source for the reference clock.
             */
            template <std::uint32_t Value>
            using aux_src_bits = ctrl_reg::Bits<AuxSrcBitField<Value>>;

            /**
            * @brief The clock source bits as a BitField.
            *
            * @tparam Value The value of the src for the reference clock.
            */
            template <std::uint32_t Value>
            struct SrcBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0x02U, "Invalid value for the reference clock.");

                /**
                 * @brief The values to which the reference clock source can be set.
                 *
                 * Values:
                 * - 0x00 ROSC
                 * - 0x01 AUXSRC
                 * - 0x02 XOSC
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The src bits.
             *
             * @tparam Value The value of the src for the reference clock.
             */
            template <std::uint32_t Value>
            using src_bits = ctrl_reg::Bits<SrcBitField<Value>>;
        };

        /**
         * @brief The div register of the reference clock.
         */
        struct div {
            static constexpr std::uintptr_t addr = clocks_base + ref_base_offset + div_offset; /**< @brief The addr of the div register of the reference clock. */
            using div_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The div register. */

            /**
             * @brief The integer component of the clock divisor.
             *
             * @tparam Value The value to which the integer part is to be set.
             */
            template <std::uint8_t Value>
            struct IntBitField {
                using reg = div_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static constexpr T position = 0x08U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0x03U, "Invalid value for the int bits.");

                /**
                 * @brief The value to which the integer component is to be set.
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The int bits of the reference clock.
             *
             * @tparam Value The value to which the integer component is to be set.
             */
            template <std::uint8_t Value>
            using int_bits = div_reg::Bits<IntBitField<Value>>;
        };

        /**
         * @brief The selected register contains information about the selected clock.
         */
        struct selected {
            static constexpr std::uintptr_t addr = clocks_base + ref_base_offset + selected_offset; /**< @brief The addr of the selected register of the reference clock. */
            using selected_reg = utils::reg_access::Reg<addr, utils::reg_access::read_access, std::uint32_t>; /**< @brief The selected register. */

            /**
             * @brief The selected bits contain information about the selected clock in the src bits of the ctrl register.
             */
            struct SelectedBitField {
                using reg = selected_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0xFFFFFFFFU << position); /**< @brief The mask of the bits. */

                /**
                 * @brief Each clock corresponds to a bit in the selected register.
                 */
                enum class value : T {
                    ROSC = (1U << 0U), /**< @brief The ROSC bit. */
                    AUX = (1U << 1U), /**< @brief The auxiliary source bit. */
                    XOSC = (1U << 2U), /**< @brief The XOSC bit. */
                };
            };

            /**
             * @brief The selected bits of the selected register.
             */
            using selected_bits = selected_reg::Bits<SelectedBitField>;
        };
    };

    /** @}*/ //rp2040_clocks
} // rp2040::system::clocks

#endif // RP2040_SYSTEM_CLOCKS_DEF_H
