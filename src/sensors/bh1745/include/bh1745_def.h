/**
 * @file bh1745_def.h
 * @brief Definitions of the firmware to use a bh1745 sensor.
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

#ifndef BH1745_DEF_H
#define BH1745_DEF_H

#include <cstdint>

namespace bh1745 {
    /**
     * @addtogroup bh1745
     * @{
     */

    /**
     * @brief I2C addresses of the bh1745 sensor.
     */
    enum class I2cAddress : std::uint8_t {
        standard = 0x38U,
        alternative = 0x39U,
    };

    /**@}*/
}

#endif //BH1745_DEF_H
