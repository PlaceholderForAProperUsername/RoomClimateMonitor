/**
 * @file st7789.tpp
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


#ifndef ST7789_TPP
#define ST7789_TPP

#include "st7789.h"

namespace st7789 {
    /**
     * @addtogroup st7789
     * @{
     */

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::init(const RgbInterfaceColorFormat rgb_format, const ControlInterfaceColorFormat control_format) {
        // data_cmd and chip_select are active low
        if (this->m_gpio.template set_output<s_data_cmd, true>() != status::Status::OK) {
            return status::Status::ERROR_CONFIG;
        }
        if (this->m_gpio.template set_output<s_chip_select, true>() != status::Status::OK) {
            return status::Status::ERROR_CONFIG;
        }
        this->send_cmd(commands::sw_reset);
        constexpr std::uint32_t wait_after_reset_ms {120};
        this->m_timer.busy_wait_ms_32(wait_after_reset_ms);

        this->send_cmd(commands::sleep_out);
        constexpr std::uint32_t wait_after_sleep_out_ms {5};
        this->m_timer.busy_wait_ms_32(wait_after_sleep_out_ms);

        std::array<std::uint8_t, 1> interface_pixel_format_data {static_cast<std::uint8_t>(std::to_underlying(control_format) | (std::to_underlying(rgb_format) << 4U))};
        this->send_cmd(commands::interface_pixel_format, interface_pixel_format_data);

        this->send_cmd(commands::inversion_on);

        this->fill_screen_16bit(0x0000U);

        this->send_cmd(commands::display_on);

        return status::Status::OK;
    }

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::set_column_address(const std::uint16_t start, const std::uint16_t end) {
        if ((start > end) || (start >= this->m_width) || (end >= this->m_width)) {
            return status::Status::ERROR_PARAM;
        }
        std::array<std::uint8_t, commands::set_column_address.second> data {
            static_cast<std::uint8_t>(start >> 8U), static_cast<std::uint8_t>(start & 0xFFU),
            static_cast<std::uint8_t>(end >> 8U), static_cast<std::uint8_t>(end & 0xFFU)
        };
        return this->send_cmd(commands::set_column_address, data);
    }

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::set_row_address(const std::uint16_t start, const std::uint16_t end) {
        if ((start > end) || (start >= this->m_height) || (end >= this->m_height)) {
            return status::Status::ERROR_PARAM;
        }
        std::array<std::uint8_t, commands::set_row_address.second> data {
            static_cast<std::uint8_t>(start >> 8U), static_cast<std::uint8_t>(start & 0xFFU),
            static_cast<std::uint8_t>(end >> 8U), static_cast<std::uint8_t>(end & 0xFFU)
        };
        return this->send_cmd(commands::set_row_address, data);
    }

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::draw_pixel_16bit(const std::uint16_t x, const std::uint16_t y, const std::uint16_t color) {
        if (auto status_code = this->set_column_address(x, x); status_code != status::Status::OK) {
            return status_code;
        }
        if (auto status_code = this->set_row_address(y, y); status_code != status::Status::OK) {
            return status_code;
        }
        std::array<std::uint8_t, 2> data {static_cast<std::uint8_t>(color & 0xFFU), static_cast<std::uint8_t>((color >> 8U) & 0xFFU)};
        return this->send_cmd(commands::write_memory, data);
    }

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::
    draw_rectangle_16bit(const std::uint16_t x, const std::uint16_t y, const std::uint16_t width, const std::uint16_t height, const std::uint16_t color) {
        if (((x + width) >= this->m_width) || (y + height >= this->m_height)) {
            return status::Status::ERROR_PARAM;
        }
        if (auto status_code = this->set_column_address(x, x + width - 1); status_code != status::Status::OK) {
            return status_code;
        }
        if (auto status_code = this->set_row_address(y, y + height - 1); status_code != status::Status::OK) {
            return status_code;
        }
        std::array<std::uint8_t, 2> color_data {
            static_cast<std::uint8_t>(color >> 8U),
            static_cast<std::uint8_t>(color & 0xFFU)
        };
        // a write memory command must be sent before a write memory continue command can be sent to ensure writing at the correct position
        if (auto status_code = this->send_cmd(commands::write_memory, color_data); status_code != status::Status::OK) {
            return status_code;
        }
        for (auto i {0U}; i < static_cast<size_t>(width * height - 1); i++) {
            if (auto status_code = this->send_cmd(commands::write_memory_continue, color_data); status_code != status::Status::OK) {
                return status_code;
            }
        }
        return status::Status::OK;
    }

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::
    draw_monocolor_16bit(const std::uint16_t x, const std::uint16_t y, const std::uint16_t width, const std::uint16_t height, const std::span<std::uint8_t> data, const std::uint16_t color) {
        constexpr auto bits_per_byte {8U};
        const auto bytes_per_row {(width / bits_per_byte) + (width % bits_per_byte > 0 ? 1 : 0) };
        // if x is greater or equal the display width, then x + data width is definitely greater than the display width, therefore no need to check if x is greater or equal the display width
        if (((x + width) >= this->m_width) || (y + height >= this->m_height) || ((height * bytes_per_row) != data.size())) {
            return status::Status::ERROR_PARAM;
        }
        for (auto row {0U}; row < height; row++) {
            for (auto column {0U}; column < width; column++) {
                const auto byte_idx {row * bytes_per_row + (column / bits_per_byte)};
                // the byte gets iterated from msb to lsb, the column goes from lsb to msb
                constexpr auto max_bit_idx_per_byte {7U};
                const auto bit_idx {max_bit_idx_per_byte - (column % bits_per_byte)};
                if (data[byte_idx] & (1U << bit_idx)) {
                    const auto target_x {x + column};
                    const auto target_y {y + row};
                    if (auto status_code = this->draw_pixel_16bit(target_x, target_y, color); status_code != status::Status::OK) {
                        return status_code;
                    }
                }
            }
        }
        return status::Status::OK;
    }

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::
    draw_bicolor_16bit(const std::uint16_t x, const std::uint16_t y, const std::uint16_t width, const std::uint16_t height, const std::span<std::uint8_t> data, const std::uint16_t fg_color, const std::uint16_t bg_color) {
        constexpr auto bits_per_byte {8U};
        const auto bytes_per_row {(width / bits_per_byte) + (width % bits_per_byte > 0 ? 1 : 0) };
        // if x is greater or equal the display width, then x + data width is definitely greater than the display width, therefore no need to check if x is greater or equal the display width
        if (((x + width) >= this->m_width) || (y + height >= this->m_height) || ((height * bytes_per_row) != data.size())) {
            return status::Status::ERROR_PARAM;
        }
        if (auto status_code = this->set_column_address(x, x + width - 1); status_code != status::Status::OK) {
            return status_code;
        }
        if (auto status_code = this->set_row_address(y, y + height - 1); status_code != status::Status::OK) {
            return status_code;
        }
        for (auto row {0U}; row < height; row++) {
            for (auto column {0U}; column < width; column++) {
                const auto byte_idx {row * bytes_per_row + (column / bits_per_byte)};
                // the byte gets iterated from msb to lsb, the column goes from lsb to msb
                constexpr auto max_bit_idx_per_byte {7U};
                const auto bit_idx {max_bit_idx_per_byte - (column % bits_per_byte)};
                std::array<std::uint8_t, 2> color {};
                // if the bit is 1, choose foreground color, else choose background color
                if (data[byte_idx] & (1U << bit_idx)) {
                    color[0] = static_cast<std::uint8_t>(fg_color & 0xFFU);
                    color[1] = static_cast<std::uint8_t>((fg_color >> 8U) & 0xFFU);
                } else {
                    color[0] = static_cast<std::uint8_t>(bg_color & 0xFFU);
                    color[1] = static_cast<std::uint8_t>((bg_color >> 8U) & 0xFFU);
                }
                // first write must be a write_memory command
                if (row == 0 && column == 0) {
                    if (auto status_code = this->send_cmd(commands::write_memory, color); status_code != status::Status::OK) {
                        return status_code;
                    }
                } else {
                    if (auto status_code = this->send_cmd(commands::write_memory_continue, color); status_code != status::Status::OK) {
                        return status_code;
                    }
                }
            }
        }
        return status::Status::OK;
    }

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::fill_screen_16bit(const std::uint16_t color) {
        if (auto status_code = this->set_column_address(0, this->m_width - 1); status_code != status::Status::OK) {
            return status_code;
        }
        if (auto status_code = this->set_row_address(0, this->m_height - 1); status_code != status::Status::OK) {
            return status_code;
        }
        std::array<std::uint8_t, 2> data {static_cast<std::uint8_t>(color & 0xFFU), static_cast<std::uint8_t>((color >> 8U) & 0xFFU)};
        for (auto column {0U}; column < this->m_width; column++) {
            for (auto row {0U}; row < this->m_height; row++) {
                if (row == 0 && column == 0) {
                    if (auto status_code = this->send_cmd(commands::write_memory, data); status_code != status::Status::OK) {
                        return status_code;
                    }
                } else {
                    if (auto status_code = this->send_cmd(commands::write_memory_continue, data); status_code != status::Status::OK) {
                        return status_code;
                    }
                }
            }
        }
        return status::Status::OK;
    }

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::send_cmd(std::pair<std::array<std::uint8_t, 1>, std::size_t> cmd, std::span<std::uint8_t> data) {
        // write memory command has a variable amount of data that can be sent, therefore it is checked extra.
        // the other commands have fixed size of parameters to be sent.
        if (((cmd == commands::write_memory || cmd == commands::write_memory_continue) && data.empty()) || ((cmd != commands::write_memory && cmd != commands::write_memory_continue) && cmd.second != data.size()))  {
            return status::Status::ERROR_PARAM;
        }
        if (this->m_gpio.template set_output<s_data_cmd, false>() != status::Status::OK) {
            return status::Status::ERROR_CONFIG;
        }
        if (this->m_gpio.template set_output<s_chip_select, false>() != status::Status::OK) {
            // resetting data_cmd pin
            this->m_gpio.template set_output<s_data_cmd, true>();
            return status::Status::ERROR_CONFIG;
        }
        this->m_spi.write_blocking(cmd.first);
        // no need to check if the output can be set for the data_cmd pin again
        this->m_gpio.template set_output<s_data_cmd, true>();
        if (cmd.second > 0) {
            this->m_spi.write_blocking(data);
        }
        // no need to check if the output can be set for the chip_select pin again
        this->m_gpio.template set_output<s_chip_select, true>();

        return status::Status::OK;
    }

    /**@}*/
}

#endif //ST7789_TPP