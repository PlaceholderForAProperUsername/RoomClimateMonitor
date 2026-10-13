/**
 * @file sgp30.h
 * @brief Firmware to use a sgp30 sensor.
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

#ifndef SGP30_H
#define SGP30_H

#include "sgp30_def.h"

namespace sgp30 {
    /**
     * @addtogroup sgp30
     * @{
     */

    /**
     * @brief The Sensirion sgp30 sensor.
     *
     * @tparam I2cControllerType Type of the I2C controller used to communicate with this sensor.
     */
    template <I2cControllerTypeConcept I2cControllerType>
    class Sgp30 {
    public:
        explicit Sgp30(I2cControllerType &i2cController) : m_controller(i2cController) {}

        /**
         * @brief Calculates the 8bit crc checksum.
         *
         * The SGP30 uses 16bit words and for every data word sent, the corresponding checksum must be added. When reading
         * data, for every 16bit word received the sgp30 will send the corresponding checksum.
         *
         * @param data The data in the format data[0] = MSB and data[1] = LSB.
         * @return std::uint8_t The crc checksum.
         */
        static std::uint8_t crc8(std::span<std::uint8_t, 2> data);
    private:
        I2cControllerType& m_controller;
        // The sgp30 has only one address
        std::uint8_t m_addr = 0x58U;

    };

    /** @}*/
}

#include "../sgp30.tpp"

#endif // SGP30_H
