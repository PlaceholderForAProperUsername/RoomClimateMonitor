/**
 * @file sgp30.tpp
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


#ifndef SGP30_TPP
#define SGP30_TPP
namespace sgp30 {
    /**
     * @addtogroup sgp30
     * @{
     */

    template <I2cControllerTypeConcept I2cControllerType>
    std::uint8_t Sgp30<I2cControllerType>::crc8(const std::span<std::uint8_t, 2> data) {
        std::uint8_t crc {0xFFU};
        const std::uint8_t polynomial {0x31U};

        for (size_t i {0}; i < data.size(); ++i) {
            crc ^= data[i];
            for (size_t j {0}; j < 8; ++j) {
                if (crc & 0x80U) {
                    crc = (crc << 1) ^ polynomial;
                } else {
                    crc = (crc << 1);
                }
            }
        }
        return crc;
    }

    /** @}*/
}
#endif // SGP30_TPP