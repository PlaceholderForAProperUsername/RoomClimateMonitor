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

#include <bitset>

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
    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    class St7789 {
    public:
        St7789() : m_flags(0) {}

        /**
         * @brief Initializes the firmware and sensor.
         *
         * The sensor is put in an initialized state by sending a software reset command. This command may require up to
         * 120ms before a sleep out command can be sent. To be on the safe side, the init function waits the 120ms.
         *
         * @return status::Status The status code.
         * @retval OK The firmware and sensor are initialized.
         * @retval ERROR_CONFIG Either the data_cmd or chip_select gpio pin is not configured as output.
         */
        status::Status init();

        /**
         * @brief Send a command to the display.
         *
         * @param cmd The command. @see commands
         * @param data The data for commands that requires it.
         * @return status::Status The status code.
         * @retval status::Status::OK The command was successfully sent.
         * @retval status::Status::ERROR_PARAM Invalid data length.
         */
        status::Status send_cmd(std::pair<std::array<std::uint8_t, 1>, std::size_t> cmd, std::span<std::uint8_t> data = std::span<std::uint8_t>());

    private:
        SpiType& m_spi {SpiType::get_instance()};
        TimerType& m_timer {TimerType::get_instance()};
        GpioType& m_gpio {GpioType::get_instance()};

        static constexpr GpioPinType s_data_cmd {data_cmd};
        static constexpr GpioPinType s_chip_select {chip_select};

        enum class Flags : std::uint32_t {
            initialized = 0x00U,
            number_of_flags
        };

        std::bitset<std::to_underlying(Flags::number_of_flags)> m_flags;
    };

    /**@}*/
}

#include "../st7789.tpp"

#endif //ST7789_H
