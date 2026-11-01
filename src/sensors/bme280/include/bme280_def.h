/**
 * @file bme280_def.h
 * @brief Definitions of the Firmware to use a bme280 sensor.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 31.10.2026
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

#ifndef BME280_DEF_H
#define BME280_DEF_H

#include <cstdint>
#include <utility>

namespace bme280 {
    /**
     * @addtogroup bme280
     * @{
     */

    /**
     * @brief The I2C addresses of the bme280.
     */
    enum class I2cAddress : std::uint8_t {
        standard = 0x76U, /**< @brief Default address of the sensor used. */
        alternative = 0x77U /**< @brief Alternative address of the sensor. */
    };

    /**
     * @brief The registers of the bme280.
     *
     * The registers are merged to blocks, as there are several continuous registers belonging together (e.g. calib00
     * to calib25, measurement data, etc.). The format is std::pair<start_address, length> to describe a register block.
     */
    namespace registers {
        constexpr std::pair<std::uint8_t, std::size_t> calib_block_0 {0x88U, 26U}; /**< @brief Calibration registers 0 to 25. */
        constexpr std::pair<std::uint8_t, std::size_t> id {0xD0U, 1U}; /**< @brief Chip id register. */
        constexpr std::pair<std::uint8_t, std::size_t> reset {0xE0U, 1U}; /**< @brief The reset register. */
        constexpr std::pair<std::uint8_t, std::size_t> calib_block_1 {0xE1U, 16U}; /**< @brief Calibration register 26 to 41. */
        constexpr std::pair<std::uint8_t, std::size_t> ctrl_hum {0xF2U, 1U}; /**< @brief Controls humidity measurement. */
        constexpr std::pair<std::uint8_t, std::size_t> status {0xF3U, 1U}; /**< @brief The status register. */
        constexpr std::pair<std::uint8_t, std::size_t> ctrl_meas {0xF4U, 1U}; /**< @brief Controls pressure and temperature measurement and set the measurement mode. */
        constexpr std::pair<std::uint8_t, std::size_t> config {0xF5U, 1U}; /**< @brief Configuration register to control standby time and iir filter. */
        constexpr std::pair<std::uint8_t, std::size_t> complete_data {0xF7U, 8U}; /**< @brief Complete measurement data registers (pressure, humidity and temperature). */
        constexpr std::pair<std::uint8_t, std::size_t> pressure_data {0xF7U, 3U}; /**< @brief Pressure measurement data registers. */
        constexpr std::pair<std::uint8_t, std::size_t> temperature_data {0xFAU, 3U}; /**< @brief Temperature measurement data registers. */
        constexpr std::pair<std::uint8_t, std::size_t> humidity_data {0xFDU, 2U}; /**< @brief Humidity measurement data registers. */
    }

    /**@}*/
}

#endif // BME280_DEF_H
