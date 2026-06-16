/**
 * @file xosc_def.h
 * @brief Provides the definitions of the RP2040's crystal oscillator (XOSC).
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 13.06.2026
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

#ifndef RP2040_SYSTEM_XOSC_DEF_H
#define RP2040_SYSTEM_XOSC_DEF_H

#include <cstdint>

#include "utils/reg_access.h"

namespace rp2040::system {
    /**
     * @addtogroup rp2040_xosc
     * @{
     */

    constexpr std::uintptr_t xosc_base = 0x40024000; /**< @brief Base address of the xosc. */

    /**
     * @brief The register map of the crystal oscillator (XOSC).
     *
     * @tparam xosc_addr
     */
    template<std::uintptr_t xosc_addr>
    struct XOSC_RegMapType {
        static_assert(xosc_addr == xosc_base, "Template parameter 'xosc_addr' must be a valid xosc address");

        constexpr static std::uint32_t ctrl_offset = 0x00; /**< @brief Offset of the ctrl register from the base address. */
        constexpr static std::uint32_t status_offset = 0x04; /**< @brief Offset of the status register from the base address.  */
        constexpr static std::uint32_t dormant_offset = 0x08; /**< @brief Offset of the dormant register from the base address.  */
        constexpr static std::uint32_t startup_offset = 0x0c; /**< @brief Offset of the startup register from the base address. */
        constexpr static std::uint32_t count_offset = 0x1C; /**< @brief Offset of the count register from the base address. */

    /**
     * @brief The xosc's ctrl register.
     */
    struct ctrl {
        constexpr static std::uintptr_t addr = xosc_addr + ctrl_offset; /**< @brief The address of the ctrl register. */
        using ctrl_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The ctrl register. */

        /**
         * @brief The enable bits as a BitField.
         *
         * The enable bits start or stop the xosc.
         */
        struct EnableBitField {
            using reg = ctrl_reg; /**< @brief The register of which the BitField is part of. */
            using T = reg::RegType; /**< @brief The data type of the register. */

            static constexpr T position = 12U; /**< @brief The position at which the BitField start in the register. */
            static constexpr T mask = (0xFFFU << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The valid values of the enable bits.
             */
            enum class value : T {
                enable = 0xfab, /**< @brief Enables the xosc */
                disable = 0xd1e, /**< @brief Disables the xosc */
            };
        };
        using enable_bits = ctrl_reg::template Bits<EnableBitField>; /**< @brief The full EnableBitField type. */

        /**
         * @brief The freq_range bits.
         *
         * Sets the frequency range of the XOSC. Currently, there is only one option, making this BitField optional.
         */
        struct FreqRangeBitField {
            using reg = ctrl_reg; /**< @brief The register of which the BitField is part of. */
            using T = reg::RegType; /**< @brief The data type of the register. */

            static constexpr T position = 0U; /**< @brief The position at which the BitField start in the register. */
            static constexpr T mask = (0xFFFU << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The valid values of the freq_range bits.
             */
            enum class value : T {
                range_1_15_mhz = 0xaa0, /**< @brief Sets the frequency range for the xosc. */
            };
        };
        using freq_range_bits = ctrl_reg::template Bits<FreqRangeBitField>; /**< The FreqRangeBitField type. */
    };

    /**
     * @brief The status register.
     */
    struct status {
        constexpr static std::uint32_t addr = xosc_addr + status_offset; /**< @brief Address of the status register. */
        using status_reg = utils::reg_access::Reg<addr, utils::reg_access::reg_mixed_access, std::uint32_t>; /**< @brief The status register type. */

        /**
         * @brief The stable bit.
         *
         * The stable bit indicates if the xosc is running and stable or not.
         */
        struct StableBitField {
            using reg = status_reg; /**< @brief The register of which the BitField is part of. */
            using T = reg::RegType; /**< @brief The data type of the register. */

            static constexpr T position = 31U; /**< @brief The position at which the BitField start in the register. */
            static constexpr T mask = (0x01U << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The values of the stable bit.
             */
            enum class value : T {
                not_stable = 0x00U, /**< @brief The xosc is either not running or not stable. */
                stable = 0x01U, /**< @brief The xosc is running and stable */
            };
        };
        using stable_bits = status_reg::template Bits<StableBitField, utils::reg_access::read_access>; /**< The StableBitField type. */

        /**
         * @brief The badwrite bit.
         *
         * This is bit is set if an invalid value has been written to enable or freq_range of the ctrl register or to the
         * dormant register. This is a WC bit.
         */
        struct BadWriteBitField {
            using reg = status_reg; /**< @brief The register of which the BitField is part of. */
            using T = reg::RegType; /**< @brief The data type of the register. */

            static constexpr T position = 24U; /**< @brief The position at which the BitField start in the register. */
            static constexpr T mask = (0x01U << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The values of the badwrite bit.
             */
            enum class value : T {
                no_bad_write = 0x00U, /**< @brief No invalid value has been written. */
                bad_write = 0x01U, /**< @brief An invalid value has been written. */
                clear_bad_write = 0x01U /**< @brief Value to clear the bit. */
            };
        };
        using bad_write_bits = status_reg::template Bits<BadWriteBitField, utils::reg_access::read_write_access>; /**< The BadWriteBitField type. */

        /**
         * @brief The enabled bit.
         */
        struct EnabledBitField {
            using reg = status_reg; /**< @brief The register of which the BitField is part of. */
            using T = reg::RegType; /**< @brief The data type of the register. */

            static constexpr T position = 12U; /**< @brief The position at which the BitField start in the register. */
            static constexpr T mask = (0x01U << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The values of the enabled bit.
             */
            enum class value : T {
                disabled = 0x00U, /**< @brief The xosc is disabled. */
                enabled = 0x01U, /**< @brief The xosc is enabled, but may be not running or stable. */
            };
        };
        using enabled_bits = status_reg::template Bits<EnabledBitField, utils::reg_access::read_access>; /**< @brief The EnabledBitField type. */

        /**
         * @brief The freq_range status bits.
         *
         * As there is only one frequency range supported, these bits are currently not needed.
         */
        struct FreqRangeBitField {
            using reg = status_reg; /**< @brief The register of which the BitField is part of. */
            using T = reg::RegType; /**< @brief The data type of the register. */

            static constexpr T position = 0U; /**< @brief The position at which the BitField start in the register. */
            static constexpr T mask = (0x03U << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The values of the freq_range bits.
             */
            enum class value : T {
                range_1_15_mhz = 0x00U, /**< @brief The selected freq_range. */
            };
        };
        using freq_range_bits  = status_reg::template Bits<FreqRangeBitField, utils::reg_access::read_access>; /**< @brief The FreqRangeBitField type. */
    }; //status

    /**
     * @brief The dormant register to pause and wake up the xosc. The pll should be stopped and an irq should be set up
     * before selecting dormant mode.
     */
    struct dormant {
        constexpr static std::uint32_t addr = xosc_addr + dormant_offset; /**< @brief The address of the dormant register. */
        using dormant_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The dormant register type. */

        /**
         * @brief The dormant bits.
         */
        struct DormantBitField {
            using reg = dormant_reg; /**< @brief The register of which the BitField is part of. */
            using T = reg::RegType; /**< @brief The data type of the register. */

            static constexpr T position = 0U; /**< @brief The position at which the BitField start in the register. */
            static constexpr T mask = (0xFFFFFFFFU << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The values of the dormant bits.
             */
            enum class value : T {
                dormant = 0x636f6d61U, /**< @brief Pauses the xosc. */
                wake = 0x77616b65U, /**< @brief Wakes up the xosc. */
            };
        };
        using dormant_bits = dormant_reg::template Bits<DormantBitField, utils::reg_access::read_write_access>; /**< The DormantBitField type. */
    }; // dormant

    /**
     * @brief The startup register controls the startup delay.
     *
     * The startup delay is specified by the delay bits multiplied by 256 in cycles. An additional 4x-multiplier can be
     * enabled with the x4 bit.
     */
    struct startup {
        constexpr static std::uintptr_t addr = xosc_addr + startup_offset; /**< @brief The address of the startup register. */
        using startup_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The startup register type. */

        constexpr static startup_reg::RegType default_delay_val = 47U; /**< @brief The default value for the startup delay according to the rp2040 datasheet. */

        constexpr static startup_reg::RegType delay_mask = 0x3FFFU; /**< @brief The mask for the delay bits. */
        constexpr static startup_reg::RegType delay_pos = 0U; /**< @brief The position of the delay bits. */

        /**
         * @brief The BitField for the delay bits.
         *
         * @tparam delay_value The value for the startup delay multiplied by 256 in cycles.
         */
        template <auto delay_value = default_delay_val>
        using DelayBitField = utils::reg_access::BitFieldValues<delay_value, startup_reg, delay_mask, delay_pos>;

        /**
         * @brief The DelayBitField type.
         *
         * @tparam delay_value The value for the startup delay multiplied by 256 in cycles.
         */
        template <auto delay_value = default_delay_val>
        using delay_bits = startup_reg::template Bits<DelayBitField<delay_value>>;

        constexpr static startup_reg::RegType x4_pos = 20U; /**< @brief The position of the x4 bit. */

        using X4BitField = utils::reg_access::BitFieldEnableDisable<startup_reg, x4_pos>; /**< @brief The BitField of the x4 bit. */

        using x4_bits = startup_reg::template Bits<X4BitField>; /**< @brief The X4BitField type.*/
    }; // startup

        /**
         * @brief The count register.
         *
         * Provided a count-down timer running as the xosc frequency.
         */
        struct count {
            constexpr static std::uintptr_t addr = xosc_addr + count_offset; /**< @brief The address of the count register. */
            using count_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The count register type. */

            constexpr static count_reg::RegType count_position = 0U; /**< @brief The position of the count bits. */
            constexpr static count_reg::RegType count_mask = (0xFFU << count_position); /**< @brief The mask of the count bits. */

            /**
             * @brief The BitField for the count bits.
             *
             * @tparam count_value The start value for the counter.
             */
            template <auto count_value>
            using CountBitField = utils::reg_access::BitFieldValues<count_value, count_reg, count_mask, count_position>;

            /**
             * @brief The CountBitField type.
             *
             * @tparam count_value The start value for the counter.
             */
            template <auto count_value>
            using count_bits = count_reg::template Bits<CountBitField<count_value>>;
        }; // count

    };
    /** @}*/ // rp2040_xosc
} //rp2040::system

#endif //RP2040_SYSTEM_XOSC_DEF_H
