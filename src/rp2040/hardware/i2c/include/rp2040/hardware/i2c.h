/**
 * @file i2c.h
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

#ifndef RP2040_HARDWARE_I2C_H
#define RP2040_HARDWARE_I2C_H

#include <bitset>

#include "i2c_def.h"

namespace rp2040::hardware::i2c {
    /**
     * @addtogroup rp2040_i2c
     * @{
     */

    template <std::uintptr_t base>
    class I2cType {
        static_assert((base == address::i2c0_base) || (base == address::i2c1_base), "Wrong i2c base address!");
    public:
        static I2cType& get_instance();

        void init();

    private:
        using Registers = I2CRegMap<base>;

        I2cType() : m_flags(0) {};

        enum class Flags : std::uint32_t {
            initialized = 0U,
            number_of_flags
        };
        std::bitset<static_cast<std::uint32_t>(Flags::number_of_flags)> m_flags;
    };

    using I2C0 = I2cType<address::i2c0_base>;
    using I2C1 = I2cType<address::i2c1_base>;

    /** @} */ // rp2040_i2c
}

#include "../../../i2c.tpp"

#endif //RP2040_HARDWARE_I2C_H
