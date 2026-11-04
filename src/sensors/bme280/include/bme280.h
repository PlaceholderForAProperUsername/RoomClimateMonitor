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

#include <bitset>

#include "status.h"

#include "bme280_def.h"

namespace bme280 {
    /**
     * @addtogroup bme280
     * @{
     */

    /**
     * @brief The parameters to configure the bme280 sensor.
     */
    struct Config {
        OversamplingFactor temperature {OversamplingFactor::disabled};
        OversamplingFactor humidity {OversamplingFactor::disabled};
        OversamplingFactor pressure {OversamplingFactor::disabled};
        FilterCoefficient filter_coefficient {FilterCoefficient::disabled};
        StandbyTime time_ms {StandbyTime::ms_1000}; // takes only effect when measurement mode is set to normal
        MeasurementMode mode {MeasurementMode::sleep};
    };

    /**
     * @brief The BME280 sensor to measure temperature, humidity and pressure.
     *
     * @tparam address The i2c address of the sensor.
     * @tparam I2cControllerType The type of the I2C controller.
     */
    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    class Bme280 {
    public:
        explicit Bme280(const I2cControllerType& i2c) : m_i2c(i2c), m_flags(0), m_compensation_parameters()  {}

        /**
         * @brief Checks if the sensor is present and responding.
         *
         * @return status::Status The status code
         * @retval status::Status::OK The sensor is present and responding.
         * @retval status::Status::ERROR_RESOURCE The sensor is either not present or not responding.
         */
        status::Status probe();

        /**
         * @brief Initializes the firmware by verifying the presence of the sensor via @see probe and getting the sensor
         * specific compensation parameters.
         *
         * @return status::Status The status code.
         * @retval status::Status::OK The firmware is initialized
         * @retval status::Status::ERROR_RESOURCE The sensor is either not present or not responding.
         */
        status::Status init();

        /**
         * @brief Configures the sensor.
         *
         * @param config The configuration parameter of the sensor.
         * @return status::Status The status code
         * @retval status::Status::OK The sensor has been configured.
         * @retval status::Status::ERROR_INIT The firmware has not been initialized. @see init
         */
        status::Status configure(Config config);

    private:
        static constexpr std::uint8_t s_addr {std::to_underlying(address)};
        I2cControllerType m_i2c;

        enum class Flags : std::uint8_t {
            initialized = 0U,
            hum_meas_enabled = 1U,
            temp_meas_enabled = 2U,
            press_meas_enabled = 3U,
            number_of_flags
        };

        std::bitset<std::to_underlying(Flags::number_of_flags)> m_flags;

        /**
         * @brief Compensation parameters needed to calculate the correct measurement value.
         *
         * This values are sensor specific. Due to the amount af parameters, theses values will be read once during firmware
         * initialization and stored in these structs.
         */
        struct CompensationParameters {
            struct Temperature {
                std::uint16_t t1;
                std::int16_t t2;
                std::int16_t t3;
            };
            Temperature temperature;
            struct Pressure {
                std::uint16_t p1;
                std::int16_t p2;
                std::int16_t p3;
                std::int16_t p4;
                std::int16_t p5;
                std::int16_t p6;
                std::int16_t p7;
                std::int16_t p8;
                std::int16_t p9;
            };
            Pressure pressure;
            struct Humidity {
                std::uint8_t h1;
                std::int16_t h2;
                std::uint8_t h3;
                std::int16_t h4;
                std::int16_t h5;
                std::int8_t h6;
            };
            Humidity humidity;
        };
        CompensationParameters m_compensation_parameters;

        /**
         * @brief Compensates the raw temperature data and returns it in °C.
         *
         * The resolution is 0.01 °C, meaning a return value of "5123" equals 51.23 °C.
         *
         * @param raw_temperature The raw temperature as read from the sensor.
         * @return std::int32_t The compensated temperature data in °C.
         */
        std::int32_t compensate_temperature(std::int32_t raw_temperature);
    };

    /**@}*/
}

#include "../bme280.tpp"

#endif // BME280_H
