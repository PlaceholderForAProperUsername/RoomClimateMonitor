/**
 * @file bme280.h
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

#ifndef BME280_H
#define BME280_H

#include "bme280_def.h"

namespace bme280 {
    /**
     * @addtogroup bme280
     * @{
     */

    /**
     * @brief The BME280 sensor to measure temperature, humidity and pressure.
     *
     * @tparam address The i2c address of the sensor.
     * @tparam I2cControllerType The type of the I2C controller.
     */
    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    class Bme280 {
    public:
        explicit Bme280(const I2cControllerType& i2c) : m_i2c(i2c) {}
    private:
        static constexpr std::uint8_t s_addr {std::to_underlying(address)};
        I2cControllerType m_i2c;
    };

    /**@}*/
}

#include "../bme280.tpp"

#endif // BME280_H
