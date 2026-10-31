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

    template <I2cControllerTypeConcept I2cControllerType, TimerTypeConcept TimerType>
    std::uint8_t Sgp30<I2cControllerType, TimerType>::crc8(const std::span<std::uint8_t, 2> data) {
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

    template <I2cControllerTypeConcept I2cControllerType, TimerTypeConcept TimerType>
    status::Status Sgp30<I2cControllerType, TimerType>::probe() {
        auto [get_serial_cmd, delay_us] = commands::get_serial_id;
        this->m_i2c.template write<m_addr>(get_serial_cmd, false);
        this->m_timer.busy_wait_us_32(delay_us);
        std::array<std::uint8_t, 9> serial_id {};
        m_i2c.template read<m_addr>(serial_id, true);
        for (size_t i {0}; i < serial_id.size(); i = i + 3) {
            std::array<std::uint8_t, 2> segment {serial_id[i], serial_id[i + 1]};
            if (crc8(segment) != serial_id[i + 2]) {
                return status::Status::ERROR_GENERIC;
            }
        }
        return status::Status::OK;
    }

    template <I2cControllerTypeConcept I2cControllerType, TimerTypeConcept TimerType>
    void Sgp30<I2cControllerType, TimerType>::measurement_init() {
        auto [init_cmd, delay_us] = commands::sgp30_iaq_init;
        this->m_i2c.template write<m_addr>(init_cmd, true);
        this->m_timer.busy_wait_us_32(delay_us);

        this->m_flags.set(std::to_underlying(Flags::measurement_initialized));
    }

    /** @}*/
}
#endif // SGP30_TPP