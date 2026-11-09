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

    /**@}*/
}

#endif //BH1745_TPP