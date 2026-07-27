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

#include <cstdint>

namespace rp2040::system::gpio {
    /**
     * @addtogroup rp2040_gpio
     */

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

    /** @} */ // rp2040_gpio

} // rp2040::system::gpio

#endif //RP2040_SYSTEM_GPIO_DEF_H
