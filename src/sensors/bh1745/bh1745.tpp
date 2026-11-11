/**
 * @file bh1745.tpp
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

#ifndef BH1745_TPP
#define BH1745_TPP

#include "bh1745.h"

namespace bh1745 {
    /**
     * @addtogroup bh1745
     * @{
     */

    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    status::Status Bh1745<address, I2cControllerType>::probe() {
        constexpr std::array<std::uint8_t, 1> get_manufacturer_id_cmd {registers::manufacturer_id.first};
        this->m_i2c.template write<s_addr>(get_manufacturer_id_cmd, false);
        std::array<std::uint8_t, registers::manufacturer_id.second> manufacturer_id_reg {};
        this->m_i2c.template read<s_addr>(manufacturer_id_reg, true);
        constexpr std::uint8_t manufacturer_id {0xE0U};
        if (manufacturer_id_reg[0] != manufacturer_id) {
            return status::Status::ERROR_RESOURCE;
        }

        constexpr std::array<std::uint8_t, 1> get_part_id_cmd {registers::system_control.first};
        this->m_i2c.template write<s_addr>(get_part_id_cmd, false);
        std::array<std::uint8_t, registers::system_control.second> part_id_reg {};
        this->m_i2c.template read<s_addr>(part_id_reg, true);
        constexpr std::uint8_t part_id {0x0BU};
        constexpr std::uint8_t part_id_mask {0x3F};
        if ((part_id_reg[0] & part_id_mask) != part_id) {
            return status::Status::ERROR_RESOURCE;
        }
        return status::Status::OK;
    }

    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    status::Status Bh1745<address, I2cControllerType>::init() {
        if (this->probe() != status::Status::OK) {
            return status::Status::ERROR_RESOURCE;
        }
        this->reset();

        this->m_flags.reset(); // Even though the flags are reset in the reset function. In case this changes, the flags are additionally reset here
        this->m_flags.set(std::to_underlying(Flags::initialized));

        return status::Status::OK;
    }

    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    void Bh1745<address, I2cControllerType>::reset() {
        constexpr std::uint8_t reset_value = (1U << 7);
        constexpr std::array<std::uint8_t, 2> reset_cmd {registers::system_control.first, reset_value};
        this->m_i2c.template write<s_addr>(reset_cmd, true);

        this->m_flags.reset();
    }

    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    status::Status Bh1745<address, I2cControllerType>::configure(const Config& config) {
        if (!this->m_flags.test(std::to_underlying(Flags::initialized))) {
            return status::Status::ERROR_INIT;
        }

        std::array<std::uint8_t, 2> set_measurement_time_cmd {registers::mode_control_1.first, std::to_underlying(config.time)};
        this->m_i2c.template write<s_addr>(set_measurement_time_cmd, true);

        // the datasheet mentions writing 0x02 to mode_control_3 register without exactly specifying why
        constexpr std::array<std::uint8_t, 2> set_mode_control_3_cmd {registers::mode_control_3.first, 0x02U};
        this->m_i2c.template write<s_addr>(set_mode_control_3_cmd, true);

        static constexpr std::uint8_t rgbc_en_bit {1U<<4};
        std::uint8_t mode_control_2_value = std::to_underlying(config.gain) | rgbc_en_bit;
        std::array<std::uint8_t, 2> set_adc_gain_cmd {registers::mode_control_2.first, mode_control_2_value};
        this->m_i2c.template write<s_addr>(set_adc_gain_cmd, true);

        this->m_flags.set(std::to_underlying(Flags::configured));

        return status::Status::OK;
    }

    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    std::expected<RgbData, status::Status> Bh1745<address, I2cControllerType>::get_compensated_rgb_data() {
        if (!this->m_flags.test(std::to_underlying(Flags::configured))) {
            return std::unexpected(status::Status::ERROR_CONFIG);
        }
        constexpr std::array<std::uint8_t, 1> get_raw_rgb_data_cmd {registers::rgb_data.first};
        this->m_i2c.template write<s_addr>(get_raw_rgb_data_cmd, false);
        std::array<std::uint8_t, registers::rgb_data.second> raw_rgb_data {};
        this->m_i2c.template read<s_addr>(raw_rgb_data, true);
        RgbData data {};
        data.red = static_cast<std::uint16_t>(((raw_rgb_data[1] << 8) | raw_rgb_data[0]) * ColorCompensation::red);
        data.green = static_cast<std::uint16_t>(((raw_rgb_data[3] << 8) | raw_rgb_data[2]) * ColorCompensation::green);
        data.blue = static_cast<std::uint16_t>(((raw_rgb_data[5] << 8) | raw_rgb_data[4]) * ColorCompensation::blue);
        return data;
    }

    /**@}*/
}

#endif //BH1745_TPP