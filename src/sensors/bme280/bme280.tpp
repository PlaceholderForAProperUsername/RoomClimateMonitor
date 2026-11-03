/**
 * @file bme280.tpp
 * @brief Firmware to use a bme280 sensor.
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

#ifndef BME280_TPP
#define BME280_TPP

#include "bme280.h"

namespace bme280 {
    /**
     * @addtogroup bme280
     * @{
     */

    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    status::Status Bme280<address, I2cControllerType>::probe() {
        constexpr std::array<std::uint8_t, 1> cmd {registers::id.first};
        this->m_i2c.template write<s_addr>(cmd, false);
        std::array<uint8_t, registers::id.second> received_chip_id {};
        this->m_i2c.template read<s_addr>(received_chip_id, true);
        if (constexpr std::uint8_t chip_id {0x60U}; chip_id != received_chip_id[0]) {
            return status::Status::ERROR_RESOURCE;
        }
        return status::Status::OK;
    }

    /**@}*/
}

#endif // BME280_TPP