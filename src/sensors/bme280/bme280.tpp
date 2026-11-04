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

    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    status::Status Bme280<address, I2cControllerType>::init() {
        if (this->probe() != status::Status::OK) {
            return status::Status::ERROR_RESOURCE;
        }
        // the compensation parameters are split in two blocks of registers
        constexpr std::array<std::uint8_t, 1> cmd_get_calib_block_0 {registers::calib_block_0.first};
        this->m_i2c.template write<s_addr>(cmd_get_calib_block_0, false);
        std::array<std::uint8_t, registers::calib_block_0.second> calib_data_0 {};
        this->m_i2c.template read<s_addr>(calib_data_0, true);

        constexpr std::array<std::uint8_t, 1> cmd_get_calib_block_1 {registers::calib_block_1.first};
        this->m_i2c.template write<s_addr>(cmd_get_calib_block_1, false);
        std::array<std::uint8_t, registers::calib_block_1.second> calib_data_1 {};
        this->m_i2c.template read<s_addr>(calib_data_1, true);

        this->m_compensation_parameters.temperature.t1 = static_cast<std::uint16_t>(calib_data_0[1]) << 8 | calib_data_0[0];
        this->m_compensation_parameters.temperature.t2 = static_cast<std::int16_t>(calib_data_0[3]) << 8 | calib_data_0[2];
        this->m_compensation_parameters.temperature.t3 = static_cast<std::int16_t>(calib_data_0[5]) << 8 | calib_data_0[4];

        this->m_compensation_parameters.pressure.p1 = static_cast<std::uint16_t>(calib_data_0[7]) << 8 | calib_data_0[6];
        this->m_compensation_parameters.pressure.p2 = static_cast<std::int16_t>(calib_data_0[9]) << 8 | calib_data_0[8];
        this->m_compensation_parameters.pressure.p3 = static_cast<std::int16_t>(calib_data_0[11]) << 8 | calib_data_0[10];
        this->m_compensation_parameters.pressure.p4 = static_cast<std::int16_t>(calib_data_0[13]) << 8 | calib_data_0[12];
        this->m_compensation_parameters.pressure.p5 = static_cast<std::int16_t>(calib_data_0[15]) << 8 | calib_data_0[14];
        this->m_compensation_parameters.pressure.p6 = static_cast<std::int16_t>(calib_data_0[17]) << 8 | calib_data_0[16];
        this->m_compensation_parameters.pressure.p7 = static_cast<std::int16_t>(calib_data_0[19]) << 8 | calib_data_0[18];
        this->m_compensation_parameters.pressure.p8 = static_cast<std::int16_t>(calib_data_0[21]) << 8 | calib_data_0[20];
        this->m_compensation_parameters.pressure.p9 = static_cast<std::int16_t>(calib_data_0[23]) << 8 | calib_data_0[22];

        this->m_compensation_parameters.humidity.h1 = static_cast<std::uint8_t>(calib_data_0[25]); // redundant cast, only implemented to show the cast wasn't forgotten
        this->m_compensation_parameters.humidity.h2 = static_cast<std::int16_t>(calib_data_1[1]) << 8 | calib_data_1[0];
        this->m_compensation_parameters.humidity.h3 = static_cast<std::uint8_t>(calib_data_1[2]); // redundant cast, only implemented to show the cast wasn't forgotten
        this->m_compensation_parameters.humidity.h4 = static_cast<std::int16_t>(calib_data_1[3]) << 4 | (calib_data_1[4] & 0xFU);
        this->m_compensation_parameters.humidity.h5 = static_cast<std::int16_t>(calib_data_1[5]) << 4 | (calib_data_1[4] & 0xF0U) >> 4;
        this->m_compensation_parameters.humidity.h6 = static_cast<std::int8_t>(calib_data_1[6]);

        this->m_flags.set(std::to_underlying(Flags::initialized));

        return status::Status::OK;
    }

    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    status::Status Bme280<address, I2cControllerType>::configure(const Config config) {
        if (!this->m_flags.test(std::to_underlying(Flags::initialized))) {
            return status::Status::ERROR_INIT;
        }

        std::array<std::uint8_t, 2> config_humidity {registers::ctrl_hum.first, std::to_underlying(config.humidity)};
        this->m_i2c.template write<s_addr>(config_humidity, true);

        const std::uint8_t ctrl_meas_val {static_cast<std::uint8_t>(std::to_underlying(config.temperature) << 5 | std::to_underlying(config.pressure) << 2 | std::to_underlying(config.mode))};
        std::array<std::uint8_t, 2> config_measurement {registers::ctrl_meas.first, ctrl_meas_val};
        this->m_i2c.template write<s_addr>(config_measurement, true);

        const std::uint8_t config_val {static_cast<std::uint8_t>(std::to_underlying(config.time_ms) << 5 | std::to_underlying(config.filter_coefficient) << 2)};
        std::array<std::uint8_t, 2> config_config {registers::config.first, config_val};
        this->m_i2c.template write<s_addr>(config_config, true);

        if (config.humidity != OversamplingFactor::disabled) {
            this->m_flags.set(std::to_underlying(Flags::hum_meas_enabled));
        }
        if (config.temperature != OversamplingFactor::disabled) {
            this->m_flags.set(std::to_underlying(Flags::temp_meas_enabled));
        }
        if (config.pressure != OversamplingFactor::disabled) {
            this->m_flags.set(std::to_underlying(Flags::press_meas_enabled));
        }

        return status::Status::OK;
    }

    /**
     * @internal
     * This function was adapted from the BME280 datasheet as provided by Bosch Sensortec. For bug fixing, consult datasheet
     * for correctness of function.
     */
    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    std::int32_t Bme280<address, I2cControllerType>::compensate_temperature(const std::int32_t raw_temperature) {
        std::int32_t tmp1 = (((raw_temperature >> 3U) - (static_cast<std::int32_t>(this->m_compensation_parameters.temperature.t1) << 1U)) *
            static_cast<std::int32_t>(this->m_compensation_parameters.temperature.t2)) >> 11U;
        std::int32_t tmp2 = ((((raw_temperature >> 4U) - (static_cast<std::int32_t>(this->m_compensation_parameters.temperature.t1))) *
            ((raw_temperature >> 4U) - (static_cast<std::int32_t>(this->m_compensation_parameters.temperature.t1))) >> 12U) *
                (static_cast<std::int32_t>(this->m_compensation_parameters.temperature.t3))) >> 14U;
        this->m_temperature_compensation = tmp1 + tmp2;
        std::int32_t temperature = ((this->m_temperature_compensation) * 5 + 128) >> 8U;

        return temperature;
    }

    /**
     * @internal
     * This function was adapted from the BME280 datasheet as provided by Bosch Sensortec. For bug fixing, consult datasheet
     * for correctness of function.
     */
    template <I2cAddress address, I2cControllerTypeConcept I2cControllerType>
    std::uint32_t Bme280<address, I2cControllerType>::compensate_pressure(const std::int32_t raw_pressure) {
        std::int64_t tmp1 = static_cast<std::int64_t>(this->m_temperature_compensation) - 128'000;
        std::int64_t tmp2 = tmp1 * tmp1 * static_cast<std::int64_t>(this->m_compensation_parameters.pressure.p6);
        tmp2 = tmp2 + ((tmp1 * static_cast<std::int64_t>(this->m_compensation_parameters.pressure.p5)) << 17U);
        tmp2 = tmp2 + ((static_cast<std::int64_t>(this->m_compensation_parameters.pressure.p4)) << 35U);
        tmp1 = ((tmp1 * tmp1 * static_cast<std::int64_t>(this->m_compensation_parameters.pressure.p3)) >> 8U) +
            ((tmp1 * static_cast<std::int64_t>(this->m_compensation_parameters.pressure.p2)) << 12U);
        tmp1 = (((static_cast<std::int64_t>(1) << 47U) + tmp1) * static_cast<std::int64_t>(this->m_compensation_parameters.pressure.p1)) >> 33U;
        if (tmp1 == 0) {
            return 0; // avoid exception caused by division by zero
        }
        std::int64_t pressure = 1'048'576 - raw_pressure;
        pressure = (((pressure << 31U) - tmp2) * 3'125) / tmp1;
        tmp1 = (static_cast<std::int64_t>(this->m_compensation_parameters.pressure.p9) * (pressure >> 13U) * (pressure >> 13U)) >> 25U;
        tmp2 = (static_cast<std::int64_t>(this->m_compensation_parameters.pressure.p8) * pressure) >> 19U;
        pressure = ((pressure + tmp1 + tmp2) >> 8U) + (static_cast<std::int64_t>(this->m_compensation_parameters) << 4U);

        return static_cast<std::uint32_t>(pressure);
    }

    /**@}*/
}

#endif // BME280_TPP