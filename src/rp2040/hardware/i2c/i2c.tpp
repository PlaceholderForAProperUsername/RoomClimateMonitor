/**
 * @file i2c.tpp
 * @brief The I2C module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 02.10.2026
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

#ifndef RP2040_HARDWARE_I2C_T
#define RP2040_HARDWARE_I2C_T

#include "include/rp2040/hardware/i2c.h"

namespace rp2040::hardware::i2c {
    /**
     * @addtogroup rp2040_i2c
     * @{
     */

    template <std::uintptr_t base>
    I2cType<base>& I2cType<base>::get_instance() {
        static I2cType instance;
        return instance;
    }

    /** @} */ // rp2040_i2c
}

#endif //RP2040_HARDWARE_I2C_T