/**
 * @file gpio_def.h
 * @brief The definitions for the GPIO module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 26.07.2026
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

#ifndef RP2040_SYSTEM_GPIO_DEF_H
#define RP2040_SYSTEM_GPIO_DEF_H

#include <complex.h>
#include <cstdint>
#include "utils/reg_access.h"

namespace rp2040::system::gpio {
    /**
     * @addtogroup rp2040_gpio
     */

    constexpr std::uintptr_t IO_BANK0_BASE = 0x40014000U; /**< @brief Base address of the user bank IO registers. */

    /**
     * @brief The GPIOs of the RP2040.
     */
    enum class GPIO : std::uint8_t {
        GPIO0 = 0U,
        GPIO1 = 1U,
        GPIO2 = 2U,
        GPIO3 = 3U,
        GPIO4 = 4U,
        GPIO5 = 5U,
        GPIO6 = 6U,
        GPIO7 = 7U,
        GPIO8 = 8U,
        GPIO9 = 9U,
        GPIO10 = 10U,
        GPIO11 = 11U,
        GPIO12 = 12U,
        GPIO13 = 13U,
        GPIO14 = 14U,
        GPIO15 = 15U,
        GPIO16 = 16U,
        GPIO17 = 17U,
        GPIO18 = 18U,
        GPIO19 = 19U,
        GPIO20 = 20U,
        GPIO21 = 21U,
        GPIO22 = 22U,
        GPIO23 = 23U,
        GPIO24 = 24U,
        GPIO25 = 25U,
        GPIO26 = 26U,
        GPIO27 = 27U,
        GPIO28 = 28U,
        GPIO29 = 29U,
        NumberOfGPIOs
    };


    /**
     * @brief Available functions for GPIOs without clock functionality.
     *
     * These functions are available for GPIO0 - GPIO19 and GPIO26 - GPIO29.
     */
    enum class GPIO_Functions : std::uint32_t {
        SPI = 1U,
        UART = 2U,
        I2C = 3U,
        PWM = 4U,
        SIO = 5U,
        PIO0 = 6U,
        PIO1 = 7U,
        USB = 9U,
    };

    /**
     * @brief Available functions for GPIOs with clock functionality.
     *
     * These functions are available for GPIO20 - GPIO25
     */
    enum class GPIO_FunctionsWithClock : std::uint32_t {
        SPI = 1U,
        UART = 2U,
        I2C = 3U,
        PWM = 4U,
        SIO = 5U,
        PIO0 = 6U,
        PIO1 = 7U,
        CLOCK = 8U,
        USB = 9U,
    };

    /**
     * @brief General register map to configure and read the status of a GPIO of the user bank IO.
     *
     * @tparam gpio The GPIO to which the regmap belongs.
     */
    template <GPIO gpio>
    struct IO_BANK0_GPIOX_RegMap {
        static_assert(gpio < GPIO::NumberOfGPIOs, "Invalid GPIO. There are only 29 user GPIOs.");

        static constexpr std::uint8_t BytesPerGPIORegisters {8U}; /**< @brief The number of bytes required for the ctrl and status registers of a GPIO. */
        static constexpr std::uint8_t offset = static_cast<std::uint8_t>(gpio) * BytesPerGPIORegisters; /** @brief The offset of the registers of the gpio. */
        static constexpr std::uintptr_t base_addr = IO_BANK0_BASE + offset; /** @brief The base address of the registers of the gpio. */
        static constexpr std::uint8_t statusOffset = 0x00U; /** @brief The offset of the status register to the gpio base address. */
        static constexpr std::uint8_t ctrlOffset = 0x04U; /** @brief The offset of the ctrl register to the gpio base address. */

        /**
         * @brief The status register of the gpio.
         */
        struct status {
            static constexpr std::uintptr_t addr = base_addr + statusOffset; /** @brief The address of the gpio's status register. */
            using status_r = utils::reg_access::Reg<addr, utils::reg_access::read_access, std::uint32_t>; /** @brief The status register. */

            /**
             * @brief Bitfield for the status bit "interrupt to processors, after override is applied".
             */
            using IRQToProcBitfield = utils::reg_access::BitFieldEnableDisable<status_r, 26U>;
            /**
             * @brief The status bit "interrupt to processors, after override is applied".
             */
            using irqToProc_bits = status_r::template Bits<IRQToProcBitfield, utils::reg_access::read_access>;

            /**
             * @brief Bitfield for the status bit "interrupt from pad before override is applied".
             */
            using IRQFromPadBitField = utils::reg_access::BitFieldEnableDisable<status_r, 24U>;
            /**
             * @brief The status bit "interrupt from pad before override is applied".
             */
            using irqFromPad_bits = status_r::template Bits<IRQFromPadBitField, utils::reg_access::read_access>;

            /**
             * @brief Bitfield for the status bit "input signal to peripheral, after override is applied".
             */
            using IntToPeriBitField = utils::reg_access::BitFieldEnableDisable<status_r, 19U>;
            /**
             * @brief The status bit "input signal to peripheral, after override is applied".
             */
            using intToPeri_bits = status_r::template Bits<IntToPeriBitField, utils::reg_access::read_access>;

            /**
             * @brief Bitfield for the status bit "input signal from pad, before override is applied".
             */
            using IntFromPadBitField = utils::reg_access::BitFieldEnableDisable<status_r, 17U>;
            /**
             * @brief The status bit "input signal from pad, before override is applied".
             */
            using intFromPad_bits = status_r::template Bits<IntFromPadBitField, utils::reg_access::read_access>;

            /**
             * @brief Bitfield for the status bit "output enable to pad after register override is applied".
             */
            using OEToPadBitField = utils::reg_access::BitFieldEnableDisable<status_r, 13U>;
            /**
             * @brief The status bit "output enable to pad after register override is applied".
             */
            using oeToPad_bits = status_r::template Bits<OEToPadBitField, utils::reg_access::read_access>;

            /**
             * @brief Bitfield for the status bit "output enable from selected peripheral, before register override is applied".
             */
            using OEFromPeriBitField = utils::reg_access::BitFieldEnableDisable<status_r, 12U>;
            /**
             * @brief The status bit "output enable from selected peripheral, before register override is applied".
             */
            using oeFromPeri_bits = status_r::template Bits<OEFromPeriBitField, utils::reg_access::read_access>;

            /**
             * @brief Bitfield for the status bit "output signal to pad after register override is applied".
             */
            using OutToPadBitField = utils::reg_access::BitFieldEnableDisable<status_r, 9U>;
            /**
             * @brief The status bit "output signal to pad after register override is applied".
             */
            using outToPad_bits = status_r::template Bits<OutToPadBitField, utils::reg_access::read_access>;

            /**
             * @brief Bitfield for the status bit "output signal from selected peripheral, before register override is applied".
             */
            using OutFromPeriField = utils::reg_access::BitFieldEnableDisable<status_r, 8U>;
            /**
             * @brief The status bit "output signal from selected peripheral, before register override is applied".
             */
            using outFromPeri_bits = status_r::template Bits<OutFromPeriField, utils::reg_access::read_access>;
        };
    };

    /** @} */ // rp2040_gpio

} // rp2040::system::gpio

#endif //RP2040_SYSTEM_GPIO_DEF_H
