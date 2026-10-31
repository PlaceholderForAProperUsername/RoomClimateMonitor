/**
 * @file sgp30_def.h
 * @brief Definitions of the Firmware to use a sgp30 sensor.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 12.10.2026
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

#ifndef SGP30_DEF_H
#define SGP30_DEF_H

#include <array>
#include <cstdint>

namespace sgp30 {
    /**
     * @addtogroup sgp30
     * @{
     */

    /**
     * @brief Checks if the I2C controller type is valid
     */
    template <typename I2cControllerType>
    concept I2cControllerTypeConcept =
        std::is_class_v<I2cControllerType> &&
        requires(I2cControllerType controller, std::array<std::uint8_t, 2> dummy)
    {
        controller.template write<0x42U>(dummy, true);
        controller.template read<0x42U>(dummy, true);
    };

    /**
     * @brief Checks if the timer type provides the requirement functions.
     */
    template <typename TimerType>
    concept TimerTypeConcept =
        std::is_class_v<TimerType> &&
        requires(TimerType timer, std::uint32_t delay)
    {
        timer.busy_wait_us_32(delay);
    };

    /**
     * @brief Implemented commands.
     *
     * Each command needs time to process before the sgp30 can send a response. The command is therefore in the format
     * std::pair<cmd, delay>.
     */
    namespace commands {
        constexpr std::pair<std::array<std::uint8_t, 2>, std::uint32_t> sgp30_iaq_init {{0x20U, 0x03U}, 10'000}; /**< @brief Starts the measurement process. */
        constexpr std::pair<std::array<std::uint8_t, 2>, std::uint32_t> sgp30_measure_iaq {{0x20U, 0x08U}, 12'000}; /**< @brief Gets measurement data. */
        constexpr std::pair<std::array<std::uint8_t, 2>, std::uint32_t> get_serial_id {{0x36U, 0x82U}, 500}; /**< @brief Gets the serial id of the sensor. */
    };

    /** @}*/
}

#endif // SGP30_DEF_H
