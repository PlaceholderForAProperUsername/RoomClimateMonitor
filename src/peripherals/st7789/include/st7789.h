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
     * @tparam data_cmd The data_command gpio pin to select whether data or a command is sent.
     * @tparam chip_select The chip select gpio pin to select the display.
     */
    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    class St7789 {
    public:
        St7789(const std::uint16_t height, const std::uint16_t width) : m_height(height), m_width(width) {}

        /**
         * @brief Initializes the display.
         *
         * Note: The init function enables display inversion, as inverting the colors of the display makes the colors
         * behave normally, e.g. setting a pixel color to 0x0000U is black and 0xFFFFU is white. While disabling the display
         * inversion sets 0x0000U to white and 0xFFFFU to black.
         *
         * @return status::Status The status code.
         * @retval status::Status::OK The display is initialized.
         * @retval status::Status::ERROR_CONFIG Either the data_cmd or chip_select gpio pin is not configured as output.
         */
        status::Status init(RgbInterfaceColorFormat rgb_format, ControlInterfaceColorFormat control_format);

        /**
         * @brief Converts rgb data to 16bit rgb565 format used by the display.
         *
         * To get the correct color, display inversion must be considered. To get the red color with display inversion on,
         * the function needs to be called a follows: convert_rgb_to_rgb565(255, 0, 0);
         * To get the red color with display inversion off: convert_rgb_to_rgb565(0, 255, 255);
         *
         * @param red The red component of the color.
         * @param green The green component of the color.
         * @param blue The blue component of the color.
         * @return std::uint16_t The color in rgb565 format.
         */
        std::uint16_t convert_rgb_to_rgb565(std::uint8_t red, std::uint8_t green, std::uint8_t blue);

        /**
         * @brief Sets the column address.
         *
         * @param start Address of the start column.
         * @param end Address of the end column.
         * @return status::Status The status code.
         * @retval status::Status::OK The column address was set successfully.
         * @retval status::Status::ERROR_PARAM Either start > end or start > display width or end > display width
         * Furthermore, this function propagates the error codes from @see send_cmd directly.
         */
        status::Status set_column_address(std::uint16_t start, std::uint16_t end);

        /**
         * @brief Sets the row address.
         *
         * @param start Address of the start row.
         * @param end Address of the end row.
         * @return status::Status The status code.
         * @retval status::Status::OK The row address was set successfully.
         * @retval status::Status::ERROR_PARAM Either start > end or start > display height or end > display height
         * Furthermore, this function propagates the error codes from @see send_cmd directly.
         */
        status::Status set_row_address(std::uint16_t start, std::uint16_t end);

        /**
         * @brief Draws a colored pixel.
         *
         * @param x The x coordinate of the pixel.
         * @param y The y coordinate of the pixel.
         * @param color The color of the pixel in 16bit.
         * @return status::Status The status code.
         * Furthermore, this function propagates the error codes from @see set_row_address @see set_column_address and @see send_cmd directly.
         */
        status::Status draw_pixel_16bit(std::uint16_t x, std::uint16_t y, std::uint16_t color);

        /**
         * @brief Draws a rectangle.
         *
         * @param x Starting column of the rectangle.
         * @param y Starting row of the rectangle.
         * @param width Width of the rectangle in pixels.
         * @param height Height of the rectangle in pixel.
         * @param color The color of the rectangle.
         * @return status::Status The status code.
         * @retval status::Status::OK The pattern was drawn successfully.
         * @retval status::Status::ERROR_PARAM The provided are out of bounds (x + width larger than display width, y + height greater than display height).
         * Furthermore, this function propagates the error codes of @see set_column_address @see set_row_address and @see send_cmd
         */
        status::Status draw_rectangle_16bit(std::uint16_t x, std::uint16_t y, std::uint16_t width, std::uint16_t height, std::uint16_t color);

        /**
         * @brief Draws a monocolor pattern on the display.
         *
         * The data is left-justified bit patterns, where a '1' will be drawn and a '0' will be skipped.
         * An example pattern at position x = 50 and y = 100 with following color = 0xAAAAU:
         * 101010101010
         * 101010101010
         * will be represented by the following data array:
         * std::array<std::uint8_t, 2*2> data {
         *  0b10101010, 0b10100000,
         *  0b10101010, 0b10100000,
         * };
         * The function call will be:
         * draw_monocolor_16bit(50, 100, 12, 2, data, 0xAAAAU);
         *
         * @param x Starting column.
         * @param y Starting row.
         * @param width Width in pixel.
         * @param height Height in pixe.
         * @param data Left-justified data where a bit of value '1' will be drawn and a bit of value '0' will be skipped.
         * @param color The color of the pattern.
         * @return status::Status The status code.
         * @retval status::Status::OK The pattern was drawn successfully.
         * @retval status::Status::ERROR_PARAM The provided are out of bounds (x + width larger than display width, y + height greater than display height, data is of wrong dimension).
         * Furthermore, this function propagates the status codes from @see draw_pixel_16bit
         */
        status::Status draw_monocolor_16bit(std::uint16_t x, std::uint16_t y, std::uint16_t width, std::uint16_t height, std::span<std::uint8_t> data, std::uint16_t color);

        /**
         * @brief Draws a bicolor pattern on the display.
         *
         * The data is left-justified bit patterns, where a '1' represents the foreground and a '0' represents the background.
         * An example pattern at position x = 50 and y = 100 with following colors foreground = 0x0000U and background = 0xFFFFU:
         * 101010101010
         * 101010101010
         * will be represented by the following data array:
         * std::array<std::uint8_t, 2*2> data {
         *  0b10101010, 0b10100000,
         *  0b10101010, 0b10100000,
         * };
         * The function call will be:
         * draw_bicolor_16bit(50, 100, 12, 2, data, 0x0000U, 0xFFFFU);
         *
         * @param x Starting column of the pattern.
         * @param y Starting row of the pattern.
         * @param width Width in pixel of the pattern.
         * @param height Height in pixel of the pattern.
         * @param data A flat array containing the left-justified data.
         * @param fg_color Foreground color corresponding to '1' in data.
         * @param bg_color Background color corresponding to '0' in data.
         * @return status::Status The status code.
         * @retval status::Status::OK The pattern was drawn successfully.
         * @retval status::Status::ERROR_PARAM The provided are out of bounds (x + width larger than display width, y + height greater than display height, data is of wrong dimension).
         * Furthermore, this function propagates the status codes from @see send_cmd @see set_column_address and @see set_row_address
         */
        status::Status draw_bicolor_16bit(std::uint16_t x, std::uint16_t y, std::uint16_t width, std::uint16_t height, std::span<std::uint8_t> data, std::uint16_t fg_color, std::uint16_t bg_color);

        /**
         * @brief Fills the whole screen in the color.
         *
         * @param color The color to fill the screen with.
         * @return status::Status The status code.
         * @retval status::Status::OK The screen was filled successfully.
         *
         * Errors are propagated from @see set_row_address @see set_column_address and @see send_cmd
         */
        status::Status fill_screen_16bit(std::uint16_t color);

        /**
         * @brief Send a command to the display.
         *
         * @param cmd The command. @see commands
         * @param data The data for commands that requires it.
         * @return status::Status The status code.
         * @retval status::Status::OK The command was successfully sent.
         * @retval status::Status::ERROR_PARAM Invalid data length.
         * @retval status::Status::ERROR_CONFIG Either the data_cmd or chip_select gpio pin is not configured as output.
         */
        status::Status send_cmd(std::pair<std::array<std::uint8_t, 1>, std::size_t> cmd, std::span<std::uint8_t> data = std::span<std::uint8_t>());

    private:
        SpiType& m_spi {SpiType::get_instance()};
        TimerType& m_timer {TimerType::get_instance()};
        GpioType& m_gpio {GpioType::get_instance()};

        std::uint16_t m_height;
        std::uint16_t m_width;

        static constexpr GpioPinType s_data_cmd {data_cmd};
        static constexpr GpioPinType s_chip_select {chip_select};
    };

    /**@}*/
}

#include "../st7789.tpp"

#endif //ST7789_H
