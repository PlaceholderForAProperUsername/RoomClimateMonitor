/**
 * @file resets_def.h
 * @brief Definitions for the resets controller.
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

#ifndef RP2040_SYSTEM_RESETS_DEF_H
#define RP2040_SYSTEM_RESETS_DEF_H

#include <cstdint>

#include "rp2040/reg_access.h"

namespace rp2040::system::resets {
    /**
     * @addtogroup rp2040_resets
     * @{
     */

    /**
     * @brief The base addresses for the resets module.
     */
    namespace address {
        constexpr std::uintptr_t resets = 0x4000c000; /**< @brief The base address of the reset controller. */
    }

    /**
     * @brief The subsystems controllable by the reset controller.
     *
     * The value corresponds to the bit position in the registers.
     */
    enum class Subsystems : std::uint32_t {
        ADC = 0U,
        BUSCTRL = 1U,
        DMA = 2U,
        I2C0 = 3U,
        I2C1 = 4U,
        IO_BANK0 = 5U,
        IO_QSPI = 6U,
        JTAG = 7U,
        PADS_BANK0 = 8U,
        PADS_QSPI = 9U,
        PIO0 = 10U,
        PIO1 = 11U,
        PLL_SYS = 12U,
        PLL_USB = 13U,
        PWM = 14U,
        RTC = 15U,
        SPI0 = 16U,
        SPI1 = 17U,
        SYSCFG = 18U,
        SYSINFO = 19U,
        TBMAN = 20U,
        TIMER = 21U,
        UART0 = 22U,
        UART1 = 23U,
        USBCTRL = 24U,
        NumberOfSubsystems
    };

    /**
     * @brief The registers of the resets controller.
     *
     * @tparam resets_addr The address of the resets controller.
     */
    template <std::uintptr_t resets_addr>
    struct ResetsRegMapType {
        static_assert(resets_addr == address::resets, "The resets address must be a valid address");

        static constexpr std::uint32_t reset_offset = 0x00U; /**< @brief The offset for the reset register. */
        static constexpr std::uint32_t wdsel_offset = 0x04U; /**< @brief The offset for the watchdog select (wdsel) register. */
        static constexpr std::uint32_t reset_done_offset = 0x08U; /**< @brief The offset for the reset done register. */

        /**
         * @brief The reset register of the reset controller.
         *
         * If a bit is set, the subsystem is held in reset. To use the subsystem, the reset must be deasserted by setting
         * the bit to 0.
         */
        struct Reset {
            static constexpr std::uint32_t addr = resets_addr + reset_offset; /**< @brief Address of the reset register. */
            using reset_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The reset register type */


            /**
             * @brief The BitType representing the bits of the reset register.
             *
             * @tparam subsystem The subsystem to be controlled.
             */
            template <Subsystems subsystem>
            struct ResetBitType {
                using reg = reset_reg; /**< @brief The register to which the bit belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write; /**< @brief Access specifier of the specific bit. */

                static_assert(subsystem < Subsystems::NumberOfSubsystems, "Invalid subsystem for ResetBitType!");

                static constexpr T position = static_cast<T>(subsystem); /**< @brief The position of the bits in the register */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the affected bits. */

                /**
                 * @brief The valid values for the bits.
                 */
                enum class value : T {
                    enable = 0U, /**< @brief Deassert the reset and thus enable the subsystem. */
                    disable = 1U, /**< @brief Put the subsystem in reset and thus disable it. */
                };
            };
        };

        /**
         * @brief The watchdog select register.
         *
         * If a bit is set and the watchdog fires, the subsystem will reset.
         */
        struct WDSel {
            static constexpr std::uint32_t addr = resets_addr + wdsel_offset; /**< @brief The address of the wdsel register. */
            using wdsel_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The wdsel register. */

            /**
             * @brief The bits of the watchdog select register.
             *
             * @tparam subsystem The subsystem to be selected.
             */
            template <Subsystems subsystem>
            struct WDSelBitType {
                using reg = wdsel_reg; /**< @brief The register to which the bits belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write; /**< @brief Access specifier of the specific bit. */

                static_assert(subsystem < Subsystems::NumberOfSubsystems, "Invalid subsystem for WDSelBitType!");

                static constexpr T position = static_cast<T>(subsystem); /**< @brief The position of the bits in the register */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the affected bits. */

                /**
                 * @brief The valid values for the bits.
                 */
                enum class value : T {
                    disable = 0U, /**< @brief The subsystem will be unaffected when the watchdog fires */
                    enable = 1U, /**< @brief The subsystem will be put in reset when the watchdog fires */
                };
            };
        };

        /**
         * @brief The reset_done register.
         *
         * If a bit is set for a subsystem, the reset for that subsystem is deasserted, meaning the registers of the subsystems
         * are ready to be accessed.
         */
        struct ResetDone {
            static constexpr std::uint32_t addr = resets_addr + reset_done_offset; /**< @brief The address of the reset_done register. */
            using reset_done_reg = reg_access::Reg<addr, reg_access::access_tag::read, std::uint32_t>; /**< @brief The reset_done register. */

            /**
             * @brief The bits of the reset_done register.
             *
             * @tparam subsystem The subsystem whose status is to be checked.
             */
            template <Subsystems subsystem>
            struct ResetDoneBitType {
                using reg = reset_done_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read; /**< @brief Access specifier of the specific bit. */

                static_assert(subsystem < Subsystems::NumberOfSubsystems, "Invalid subsystem!");

                static constexpr T position = static_cast<T>(subsystem); /**< @brief The position of the bits in the register */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the affected bits. */

                /**
                 * @brief The meaning of the bits.
                 */
                enum class value : T {
                    disabled = 0U, /**< @brief The subsystem is still in reset and thus disabled. */
                    enabled = 1U /**< @brief The subsystem is out of reset and thus enabled. */
                };
            };
        };
    };

    /** @}*/ // rp2040_reset
} // rp2040::system::resets


#endif // RP2040_SYSTEM_RESETS_DEF_H
