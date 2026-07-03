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
             */
            struct AuxSrcBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static constexpr T position = 0x05U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                /**
                 * @brief The values to which the auxiliary clock source can be set.
                 */
                enum class value : T {
                    PLL_USB = 0x00U, /**< @brief Set source to the USB PLL. */
                    GPIN0 = 0x01U, /**< @brief Set the source to GPIN0. */
                    GPIN1 = 0x02U, /**< @brief Set the source to GPIN1. */
                };
            };

            using aux_src_bits = ctrl_reg::Bits<AuxSrcBitField>; /**< @brief The auxiliary source bits. */

            /**
            * @brief The clock source bits as a BitField.
            */
            struct SrcBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                /**
                 * @brief The values to which the reference clock source can be set.
                 */
                enum class value : T {
                    ROSC = 0x00U, /**< @brief Sets the reference clock source to the ROSC. */
                    Aux = 0x01U, /**< @brief Sets the reference clock source to use an auxiliary source. */
                    XOSC = 0x02U, /**< @brief Sets the reference clock source to the XOSC. */
                };
            };

            using src_bits = ctrl_reg::Bits<SrcBitField>; /**< @brief The src bits. */
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
    };

    /** @}*/ //rp2040_clocks
} // rp2040::system::clocks

#endif // RP2040_SYSTEM_CLOCKS_DEF_H
