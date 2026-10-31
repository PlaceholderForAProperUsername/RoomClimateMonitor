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

#include <bitset>
#include <expected>

#include "status.h"

#include "sgp30_def.h"

namespace sgp30 {
    /**
     * @addtogroup sgp30
     * @{
     */

    /**
     * @brief The Sensirion sgp30 sensor.
     *
     * The sgp30 measures total volatile organic compounds (TVOC) and equivalent CO2 (CO2eq) within following ranges:
     *  TVOC: 0 ppb - 60,000 ppb
     *  CO2eq: 400 ppm - 60,000 ppm
     *
     * After the initializing process has finished, the measurement command should be sent every seconds to achieve the
     * best measurement results.
     *
     * @tparam I2cControllerType Type of the I2C controller used to communicate with this sensor.
     * @tparam TimerType Type of the timer used for delays.
     */
    template <I2cControllerTypeConcept I2cControllerType, TimerTypeConcept TimerType>
    class Sgp30 {
    public:
        explicit Sgp30(I2cControllerType &i2cController, TimerType &timer) : m_i2c(i2cController), m_timer(timer), m_flags(0) {}

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

        /**
         * @brief Checks if a sgp30 sensor is present and responding.
         *
         * @return status::Status The status code
         * @retval status::Status::OK The sensor is present and responding.
         * @retval statsu::Status::GENERIC The crc check of the message failed.
         */
        status::Status probe();

        /**
         * @brief Initializes the measurement process.
         *
         * The initialization phase may take up to 15s. During that time frame, the sensor returns fixed values of 400ppm
         * CO2eq and 0ppb TVOC when sending the sgp30_measure_iaq command.
         */
        void measurement_init();

        /**
         * @brief Sends the measurement command and gets the measurement result.
         *
         * @return std::expected<std::pair<std::uint16_t, std::uint16_t>, status::Status> The measurement values in case of success or an error code.
         * @retval std::pair<std::uint16_t, std::uint16_t> The measurement data in the std::pair<co2eq, tvoc> format.
         * @retval status::Status::ERROR_INIT The sgp30 sensor has not been initialized. @see measurement_init
         * @retval status::Status::ERROR_GENERIC The crc check of the data failed.
         */
        std::expected<std::pair<std::uint16_t, std::uint16_t>, status::Status> conduct_measurement();

    private:
        I2cControllerType& m_i2c; /**< @brief The I2C controller used to communicate with the sensor. */
        TimerType m_timer; /**< @brief The timer to provide needed wait functions. */

        /**
         * @brief The status flags.
         */
        enum class Flags : std::uint8_t {
            measurement_initialized = 0U,
            number_of_flags
        };

        std::bitset<std::to_underlying(Flags::number_of_flags)> m_flags; /**< @brief Status flags of the sgp30. */

        // The sgp30 has only one address, therefore it gets hardcoded
        static constexpr std::uint8_t m_addr {0x58U}; /**< @brief I2C address of the sensor. */
    };

    /** @}*/
}

#include "../sgp30.tpp"

#endif // SGP30_H
