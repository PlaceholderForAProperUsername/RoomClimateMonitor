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

#include "rp2040/reg_access.h"

namespace rp2040::system::xosc {
    /**
     * @addtogroup rp2040_xosc
     * @{
     */

    /**
     * @brief The addresses of the xosc module.
     */
    namespace address {
        constexpr std::uintptr_t xosc = 0x40024000; /**< @brief Base address of the xosc. */
    }

    /**
     * @brief The register map of the crystal oscillator (XOSC).
     *
     * @tparam xosc_addr The base address of the xosc instance.
     */
    template<std::uintptr_t xosc_addr>
    struct XoscRegMapType {
        static_assert(xosc_addr == address::xosc, "Template parameter 'xosc_addr' must be a valid xosc address");

        static constexpr std::uint32_t ctrl_offset = 0x00; /**< @brief Offset of the ctrl register from the base address. */
        static constexpr std::uint32_t status_offset = 0x04; /**< @brief Offset of the status register from the base address.  */
        static constexpr std::uint32_t dormant_offset = 0x08; /**< @brief Offset of the dormant register from the base address.  */
        static constexpr std::uint32_t startup_offset = 0x0c; /**< @brief Offset of the startup register from the base address. */
        static constexpr std::uint32_t count_offset = 0x1C; /**< @brief Offset of the count register from the base address. */

    /**
     * @brief The xosc's ctrl register.
     */
    struct Ctrl {
        static constexpr std::uintptr_t addr = xosc_addr + ctrl_offset; /**< @brief The address of the ctrl register. */
        using ctrl_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The ctrl register. */

        /**
         * @brief The enable bits.
         *
         * The enable bits start or stop the xosc.
         */
        struct EnableBitType {
            using reg = ctrl_reg; /**< @brief The register of which the bits belong. */
            using T = reg::RegType; /**< @brief The data type of the register. */
            using bit_access = reg_access::access_tag::read_write; /**< @brief Access specifier of the specific bit. */

            static constexpr T position = 12U; /**< @brief The position of the bits in the register. */
            static constexpr T mask = (0xFFFU << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The valid values of the enable bits.
             */
            enum class value : T {
                enable = 0xfabU, /**< @brief Enables the xosc */
                disable = 0xd1eU, /**< @brief Disables the xosc */
            };
        };

        /**
         * @brief The freq_range bits.
         *
         * Sets the frequency range of the XOSC. Currently, there is only one option, making this BitType optional.
         */
        struct FreqRangeBitType {
            using reg = ctrl_reg; /**< @brief The register of which the bits belong. */
            using T = reg::RegType; /**< @brief The data type of the register. */
            using bit_access = reg_access::access_tag::read_write; /**< @brief Access specifier of the specific bit. */

            static constexpr T position = 0U; /**< @brief The position of the bits in the register. */
            static constexpr T mask = (0xFFFU << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The valid values of the freq_range bits.
             */
            enum class value : T {
                range_1_15_mhz = 0xaa0U, /**< @brief Sets the frequency range for the xosc. */
            };
        };
    }; // Ctrl

    /**
     * @brief The status register.
     */
    struct Status {
        static constexpr std::uint32_t addr = xosc_addr + status_offset; /**< @brief Address of the status register. */
        using status_reg = reg_access::Reg<addr, reg_access::access_tag::mixed_access, std::uint32_t>; /**< @brief The status register type. */

        /**
         * @brief The stable bit.
         *
         * The stable bit indicates if the xosc is running and stable or not.
         */
        struct StableBitType {
            using reg = status_reg; /**< @brief The register of which the bits belong. */
            using T = reg::RegType; /**< @brief The data type of the register. */
            using bit_access = reg_access::access_tag::read; /**< @brief Access specifier of the specific bit. */

            static constexpr T position = 31U; /**< @brief The position of the bits in the register. */
            static constexpr T mask = (0x01U << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The values of the stable bit.
             */
            enum class value : T {
                not_stable = 0x00U, /**< @brief The xosc is either not running or not stable. */
                stable = 0x01U, /**< @brief The xosc is running and stable */
            };
        };

        /**
         * @brief The badwrite bit.
         *
         * This is bit is set if an invalid value has been written to enable or freq_range of the ctrl register or to the
         * dormant register.
         */
        struct BadWriteBitType {
            using reg = status_reg; /**< @brief The register of which the bits belong. */
            using T = reg::RegType; /**< @brief The data type of the register. */
            using bit_access = reg_access::access_tag::write_clear; /**< @brief Access specifier of the specific bit. */

            static constexpr T position = 24U; /**< @brief The position of the bits in the register. */
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

        /**
         * @brief The enabled bit.
         */
        struct EnabledBitType{
            using reg = status_reg; /**< @brief The register of which the bits belong. */
            using T = reg::RegType; /**< @brief The data type of the register. */
            using bit_access = reg_access::access_tag::read; /**< @brief Access specifier of the specific bit. */

            static constexpr T position = 12U; /**< @brief The position of the bits in the register. */
            static constexpr T mask = (0x01U << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The values of the enabled bit.
             */
            enum class value : T {
                disabled = 0x00U, /**< @brief The xosc is disabled. */
                enabled = 0x01U, /**< @brief The xosc is enabled, but may be not running or stable. */
            };
        };

        /**
         * @brief The freq_range status bits.
         *
         * As there is only one frequency range supported, these bits are currently not needed.
         */
        struct FreqRangeBitType{
            using reg = status_reg; /**< @brief The register of which the bits belong. */
            using T = reg::RegType; /**< @brief The data type of the register. */
            using bit_access = reg_access::access_tag::read; /**< @brief Access specifier of the specific bit. */

            static constexpr T position = 0U; /**< @brief The position of the bits in the register. */
            static constexpr T mask = (0x03U << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The values of the freq_range bits.
             */
            enum class value : T {
                range_1_15_mhz = 0x00U, /**< @brief The selected freq_range. */
            };
        };
    }; // Status

    /**
     * @brief The dormant register to pause and wake up the xosc. The pll should be stopped and an irq should be set up
     * before selecting dormant mode.
     */
    struct Dormant {
        static constexpr std::uint32_t addr = xosc_addr + dormant_offset; /**< @brief The address of the dormant register. */
        using dormant_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The dormant register type. */

        /**
         * @brief The dormant bits.
         */
        struct DormantBitType {
            using reg = dormant_reg; /**< @brief The register of which the bits belong. */
            using T = reg::RegType; /**< @brief The data type of the register. */
            using bit_access = reg_access::access_tag::read_write; /**< @brief Access specifier of the specific bit. */

            static constexpr T position = 0U; /**< @brief The position of the bits in the register. */
            static constexpr T mask = (0xFFFFFFFFU << position); /**< @brief The mask of the controlled bits. */

            /**
             * @brief The values of the dormant bits.
             */
            enum class value : T {
                dormant = 0x636f6d61U, /**< @brief Pauses the xosc. */
                wake = 0x77616b65U, /**< @brief Wakes up the xosc. */
            };
        };
    }; // dormant

    /**
     * @brief The startup register controls the startup delay.
     *
     * The startup delay is specified by the delay bits multiplied by 256 in cycles. An additional 4x-multiplier can be
     * enabled with the x4 bit.
     */
    struct Startup {
        static constexpr std::uintptr_t addr = xosc_addr + startup_offset; /**< @brief The address of the startup register. */
        using startup_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The startup register type. */

        static constexpr startup_reg::RegType default_delay_val = 47U; /**< @brief The default value for the startup delay according to the rp2040 datasheet. */

        static constexpr startup_reg::RegType delay_mask = 0x3FFFU; /**< @brief The mask for the delay bits. */
        static constexpr startup_reg::RegType delay_pos = 0U; /**< @brief The position of the delay bits. */
        /**
         * @brief The delay bits.
         *
         * @tparam delay_value The value for the startup delay multiplied by 256 in cycles.
         */
        template <auto delay_value = default_delay_val>
        using DelayBitType = reg_access::BitTypeValues<delay_value, startup_reg, delay_mask, delay_pos>;


        static constexpr startup_reg::RegType x4_pos = 20U; /**< @brief The position of the x4 bit. */
        using X4BitType = reg_access::BitTypeEnableDisable<startup_reg, x4_pos>; /**< @brief The x4 bits. */
    }; // Startup

        /**
         * @brief The count register.
         *
         * Provided a count-down timer running as the xosc frequency.
         */
        struct Count {
            static constexpr std::uintptr_t addr = xosc_addr + count_offset; /**< @brief The address of the count register. */
            using count_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The count register type. */

            static constexpr count_reg::RegType count_position = 0U; /**< @brief The position of the count bits. */
            static constexpr count_reg::RegType count_mask = (0xFFU << count_position); /**< @brief The mask of the count bits. */

            /**
             * @brief The BitField for the count bits.
             *
             * @tparam count_value The start value for the counter.
             */
            template <auto count_value>
            using CountBitType = reg_access::BitTypeValues<count_value, count_reg, count_mask, count_position>;
        }; // count

    };
    /** @}*/ // rp2040_xosc
} //rp2040::system::xosc

#endif //RP2040_SYSTEM_XOSC_DEF_H
