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
        constexpr auto cmd {commands::get_serial_id};
        this->m_i2c.template write<m_addr>(cmd.cmd, false);
        this->m_timer.busy_wait_us_32(cmd.delay_us);
        std::array<std::uint8_t, cmd.response_length> serial_id {};
        this->m_i2c.template read<m_addr>(serial_id, true);
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
        constexpr auto cmd {commands::sgp30_iaq_init};
        this->m_i2c.template write<m_addr>(cmd.cmd, true);
        this->m_timer.busy_wait_us_32(cmd.delay_us);

        this->m_flags.set(std::to_underlying(Flags::measurement_initialized));
    }

    template <I2cControllerTypeConcept I2cControllerType, TimerTypeConcept TimerType>
    std::expected<std::pair<std::uint16_t, std::uint16_t>, status::Status> Sgp30<I2cControllerType, TimerType>::conduct_measurement() {
        if (!this->m_flags.test(std::to_underlying(Flags::measurement_initialized))) {
            return std::unexpected(status::Status::ERROR_INIT);
        }
        constexpr auto cmd {commands::sgp30_measure_iaq};
        this->m_i2c.template write<m_addr>(cmd.cmd, false);
        this->m_timer.busy_wait_us_32(cmd.delay_us);
        std::array<std::uint8_t, cmd.response_length> measurement_data {};
        this->m_i2c.template read<m_addr>(measurement_data, true);
        for (size_t i {0}; i < cmd.response_length; i = i + 3) {
            std::array<std::uint8_t, 2> segment {measurement_data[i], measurement_data[i + 1]};
            if (crc8(segment) != measurement_data[i + 2]) {
                return std::unexpected(status::Status::ERROR_GENERIC);
            }
        }
        std::uint16_t co2_data = static_cast<std::uint16_t>(measurement_data[0]) << 8U | static_cast<std::uint16_t>(measurement_data[1]);
        std::uint16_t tvoc_data = static_cast<std::uint16_t>(measurement_data[3]) << 8U | static_cast<std::uint16_t>(measurement_data[4]);
        return std::make_pair(co2_data, tvoc_data);
    }

    /** @}*/
}
#endif // SGP30_TPP