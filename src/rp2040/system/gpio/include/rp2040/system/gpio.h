/**
 * @file gpio.h
 * @brief The GPIO module to use the gpios of the RP2040.
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

#ifndef RP2040_SYSTEM_GPIO_H
#define RP2040_SYSTEM_GPIO_H

#include <bitset>
#include <cstdint>

#include "gpio_def.h"


namespace rp2040::system::gpio {
    /**
     * @addtogroup rp2040_gpio
     * @{
     */

    /**
     * @brief The GPIO type to configure and use the GPIOs.
     *
     * @tparam ioBankBase The address of the User Bank IO registers.
     * @tparam padsBankBase The address of the User Bank Pad Control registers.
     * @tparam sioBase The address of the Software IO registers.
     */
    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    class GpioType {
        static_assert((ioBankBase == IO_BANK0_BASE) && (padsBankBase == PADS_BANK0_BASE) && (sioBase == SIO_BASE), "Invalid gpio base addresses.");

    public:
        /**
         * @brief Gets the Singleton instance.
         *
         * @return GpioType& The Singleton instance.
         */
        static GpioType<ioBankBase, padsBankBase, sioBase>& getInstance();

        /**
         * @brief Initializes the GPIO module by taking it out of reset.
         *
         * @pre None
         *
         * @post The GPIO is taken out of reset and the GPIO is configurable/usable.
         */
        void init();

    private:
        GpioType() : m_flags(0) {};

        enum Flags : std::uint32_t {
            Initialized = 0U,
            NumberOfFlags,
        };

        std::bitset<static_cast<std::uint32_t>(Flags::NumberOfFlags)> m_flags;
    };


    using GPIO = GpioType<IO_BANK0_BASE, PADS_BANK0_BASE, SIO_BASE>; /**< @brief The GPIO. */

    /** @} */ // rp2040_gpio
} // rp2040::system::gpio



#include "../../../gpio.tpp"

#endif //RP2040_SYSTEM_GPIO_H
