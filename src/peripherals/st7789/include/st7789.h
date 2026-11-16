/**
 * @file st7789.h
 * @brief Firmware to use a display with a st7789 driver.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 12.11.2026
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

#ifndef ST7789_H
#define ST7789_H

#include "status.h"

#include "st7789_def.h"

namespace st7789 {
    /**
     * @addtogroup st7789
     * @{
     */

    /**
     * @brief The display with a ST7789 driver.
     *
     * @tparam SpiType The type of the SPI used to communicate with the display.
     * @tparam TimerType The type of the timer providing necessary wait functions.
     * @tparam GpioType The type of the gpio providing functions to control chip select and data_command pins.
     * @tparam GpioPinType The type of the gpio pins.
     */
    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType>
    requires std::is_enum_v<GpioPinType> && std::is_class_v<GpioType> && requires (GpioType gpio)
    {
        // GpioType and GpioPinType must be checked together for working properly, therefore no concept for those types
        { gpio.template set_output<GpioPinType::gpio0, true>() } -> std::same_as<status::Status>;
        { GpioType::get_instance() } -> std::same_as<GpioType&>;
    }
    class St7789 {
    public:
        St7789() = default;

    private:
        static constexpr SpiType& m_spi {SpiType::get_instance()};
        static constexpr TimerType& m_timer {TimerType::get_instance()};
        static constexpr GpioType& m_gpio {GpioType::get_instance()};
    };

    /**@}*/
}

#include "../st7789.tpp"

#endif //ST7789_H
