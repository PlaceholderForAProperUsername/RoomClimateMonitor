/**
 * @file bh1745.h
 * @brief Firmware to use a bh1745 sensor.
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

#ifndef BH1745_H
#define BH1745_H

#include <bitset>

#include "status.h"

#include "bh1745_def.h"

namespace bh1745 {
    /**
     * @addtogroup bh1745
     * @{
     */

    struct Config {
        MeasurementTime time;
        AdcGain gain;
    };

    /**
     * @brief The BH1745 ambient light sensor.
     *
     * @tparam address The I2C address of the sensor.
     * @tparam I2cControllerType The type of the I2C controller.
     */
    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    class Bh1745 {
    public:
        explicit Bh1745(I2cControllerType& i2c_controller) : m_i2c(i2c_controller), m_flags(0) {}

        /**
         * @brief Checks if a BH1745 is present and responding.
         *
         * @return status::Status The status code.
         * @retval status::Status::OK The sensor is present and responding.
         * @retval status::Status::RESOURCE The sensor is either not responding or the sensor is not a bh1745.
         */
        status::Status probe();

        /**
         * @brief Initializes the firmware and sensor.
         *
         * @return status::Status The status code.
         * @retval status::Status::OK The firmware is initialized and ready to be configured.
         * @retval status::Status::RESOURCE The sensor is either not responding or the sensor is not a bh1745.
         */
        status::Status init();

        /**
         * @brief Sends the software reset command to the sensor and de-initializes the firmware.
         */
        void reset();

        /**
         * @brief Configures the sensor and enables measurement.
         *
         * @param config The configuration of the sensor.
         */
        void config(const Config& config);

    private:
        static constexpr std::uint8_t s_addr {std::to_underlying(address)};
        I2cControllerType& m_i2c;

        /**
         * @brief Compensation factors for the raw color readings.
         *
         * The sensors sensitivity differs for the colors.
         */
        struct ColorCompensation {
            // compensation parameters taken from Pimoroni https://github.com/pimoroni/bh1745-python/blob/main/bh1745/__init__.py
            // Pimoroni is the vendor from which I sourced the breakout board with the bh1745 sensor
            static constexpr float red {2.2f};
            static constexpr float green {1.0f};
            static constexpr float blue {1.8f};
            static constexpr float clear {10.0f};
        };

        enum class Flags : std::uint8_t {
            initialized = 0x00U,
            configured = 0x1U,
            number_of_flags
        };

        std::bitset<std::to_underlying(Flags::number_of_flags)> m_flags;
    };

    /**@}*/
}

#include "../bh1745.tpp"

#endif //BH1745_H
