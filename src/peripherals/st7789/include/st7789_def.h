/**
 * @file st7789_def.h
 * @brief Definitions of the firmware to use a display with a st7789 driver.
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
#ifndef ST7789_DEF_H
#define ST7789_DEF_H

#include <array>
#include <cstdint>
#include <utility>

namespace st7789 {
    /**
     * @addtogroup st7789
     * @{
     */

    /**
     * @brief Options for the rgb interface color format.
     */
    enum class RgbInterfaceColorFormat : std::uint8_t {
        rgb_65K = 0b101U,
        rgb_262K = 0b110U,
    };

    /**
     * @brief Options for the control interface color format.
     */
    enum class ControlInterfaceColorFormat : std::uint8_t {
        bits_per_pixel_12 = 0b011U,
        bits_per_pixel_16 = 0b101U,
        bits_per_pixel_18 = 0b110U,
    };

    /**
     * @brief Checks if the spi type provides the necessary methods.
     */
    template <typename SpiType>
    concept SpiTypeConcept =
        std::is_class_v<SpiType> &&
        requires (SpiType spi, std::array<std::uint8_t, 4U> data)
    {
        spi.write_blocking(data);
        { SpiType::get_instance() } -> std::same_as<SpiType&>;
    };

    /**
     * @brief Checks if the timer type provides the required functions.
     */
    template <typename TimerType>
    concept TimerTypeConcept =
        std::is_class_v<TimerType> &&
        requires(TimerType timer, std::uint32_t delay)
    {
        timer.busy_wait_ms_32(delay);
        { TimerType::get_instance() } -> std::same_as<TimerType&>;
    };

    /**
     * @brief Checks if the gpio and gpio pin type provides the required functions.
     */
    template <typename GpioType, typename GpioPinType>
    concept GpioTypeConcept =
        std::is_enum_v<GpioPinType> && std::is_class_v<GpioType> && requires (GpioType gpio)
    {
        { gpio.template set_output<static_cast<GpioPinType>(0), true>() } -> std::same_as<status::Status>;
        { GpioType::get_instance() } -> std::same_as<GpioType&>;
    };

    /**
     * @brief List of commands of the st7789 driver.
     *
     * The commands are in the format std::pair<cmd, size>, where size is either the number of additional bytes to be sent or read.
     */
    namespace commands {
        constexpr std::pair<std::uint8_t, std::size_t> sw_reset {0x01U, 0U}; /**< @brief Software reset. */
        constexpr std::pair<std::uint8_t, std::size_t> read_id {0x04U, 4U}; /**< @brief Read display id. */
        constexpr std::pair<std::uint8_t, std::size_t> sleep_in {0x10U, 0U}; /**< @brief Enable sleep mode. */
        constexpr std::pair<std::uint8_t, std::size_t> sleep_out {0x11U, 0U}; /**< @brief Disable sleep mode. */
        constexpr std::pair<std::uint8_t, std::size_t> partial_mode_on {0x12U, 0U}; /**< @brief Enables partial mode. */
        constexpr std::pair<std::uint8_t, std::size_t> partial_mode_off {0x13U, 0U}; /**< @brief Disables partial mode. */
        constexpr std::pair<std::uint8_t, std::size_t> inversion_off {0x20U, 0U}; /**< @brief Disables display inversion. */
        constexpr std::pair<std::uint8_t, std::size_t> inversion_on {0x21U, 0U}; /**< @brief Enables display inversion. */
        constexpr std::pair<std::uint8_t, std::size_t> gamma_set {0x26U, 1U}; /**< @brief Sets gamma values. */
        constexpr std::pair<std::uint8_t, std::size_t> display_off {0x28U, 0U}; /**< @brief Turns the display off. */
        constexpr std::pair<std::uint8_t, std::size_t> display_on {0x29U, 0U}; /**< @brief Turns the display on. */
        constexpr std::pair<std::uint8_t, std::size_t> set_column_address {0x2AU, 4U}; /**< @brief Sets the column address range (start, end). */
        constexpr std::pair<std::uint8_t, std::size_t> set_row_address {0x2BU, 4U}; /**< @brief Sets the row address range (start, end). */
        constexpr std::pair<std::uint8_t, std::size_t> write_memory {0x2CU, 1U}; /**< @brief Writes data to the memory. The size is the minimum size. */
        constexpr std::pair<std::uint8_t, std::size_t> tear_off {0x34U, 0U}; /**< @brief Disables tearing effect. */
        constexpr std::pair<std::uint8_t, std::size_t> tear_on {0x35U, 1U}; /**< @brief Enables tearing effect. */
        constexpr std::pair<std::uint8_t, std::size_t> memory_data_access_ctrl {0x36U, 1U}; /**< @brief Sets memory data access control. */
        constexpr std::pair<std::uint8_t, std::size_t> idle_mode_off {0x38U, 0U}; /**< @brief Disables idle mode. */
        constexpr std::pair<std::uint8_t, std::size_t> idle_mode_on {0x39U, 0U}; /**< @brief Enables idle mode. */
        constexpr std::pair<std::uint8_t, std::size_t> interface_pixel_format {0x3AU, 1U}; /**< @brief Sets interface pixel format. */
    }

    /**@}*/
}

#endif //ST7789_DEF_H
