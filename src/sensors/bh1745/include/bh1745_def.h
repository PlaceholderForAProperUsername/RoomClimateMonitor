/**
 * @file bh1745_def.h
 * @brief Definitions of the firmware to use a bh1745 sensor.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 08.11.2026
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

#ifndef BH1745_DEF_H
#define BH1745_DEF_H

#include <cstdint>
#include <utility>

namespace bh1745 {
    /**
     * @addtogroup bh1745
     * @{
     */

    /**
     * @brief I2C addresses of the bh1745 sensor.
     */
    enum class I2cAddress : std::uint8_t {
        standard = 0x38U,
        alternative = 0x39U,
    };

    /**
     * @brief Registers of the bh1745 sensor.
     *
     * The registers are merged to block, as there are several continuous registers belonging together (e.g. rgbc data,
     * threshold, etc.). The format is std::pair<start_address, length> to describe register blocks.
     */
    namespace registers {
        constexpr std::pair<std::uint8_t, std::size_t> system_control {0x40U, 1U}; /**< @brief System control register. */
        constexpr std::pair<std::uint8_t, std::size_t> mode_control_1 {0x41U, 1U}; /**< @brief Functional configuration register. */
        constexpr std::pair<std::uint8_t, std::size_t> mode_control_2 {0x42U, 1U}; /**< @brief Functional configuration register. */
        constexpr std::pair<std::uint8_t, std::size_t> mode_control_3 {0x44U, 1U}; /**< @brief Functional configuration register. */
        constexpr std::pair<std::uint8_t, std::size_t> rgbc_data {0x50U, 8U}; /**< @brief Red, green, blue and clear data registers. */
        constexpr std::pair<std::uint8_t, std::size_t> red_data {0x50U, 2U}; /**< @brief Red data registers, LSB first, MSB second. */
        constexpr std::pair<std::uint8_t, std::size_t> green_data {0x52U, 2U}; /**< @brief Green data registers, LSB first, MSB second. */
        constexpr std::pair<std::uint8_t, std::size_t> blue_data {0x54U, 2U}; /**< @brief Blue data registers, LSB first, MSB second. */
        constexpr std::pair<std::uint8_t, std::size_t> clear_data {0x56U, 2U}; /**< @brief Clear data registers, LSB first, MSB second. */
        constexpr std::pair<std::uint8_t, std::size_t> dint_data {0x58U, 2U}; /**< @brief Internal data registers, LSB first, MSB second. */
        constexpr std::pair<std::uint8_t, std::size_t> interrupt {0x60U, 1U}; /**< @brief Interrupt setting register. */
        constexpr std::pair<std::uint8_t, std::size_t> persistence {0x61U, 1U}; /**< @brief Persistence setting register. */
        constexpr std::pair<std::uint8_t, std::size_t> threshold_high {0x62U, 2U}; /**< @brief High threshold register, LSB first, MSB second. */
        constexpr std::pair<std::uint8_t, std::size_t> threshold_low {0x64U, 2U}; /**< @brief Low threshold register, LSB first, MSB second. */
        constexpr std::pair<std::uint8_t, std::size_t> manufacturer_id {0x92U, 1U}; /**< @brief Manufacturer ID register. */
    }

    /**@}*/
}

#endif //BH1745_DEF_H
