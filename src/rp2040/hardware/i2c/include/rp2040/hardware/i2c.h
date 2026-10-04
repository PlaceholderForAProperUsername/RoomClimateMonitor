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

#include "status.h"
#include "i2c_def.h"
#include "rp2040/system/pll_def.h"


namespace rp2040::hardware::i2c {
    /**
     * @addtogroup rp2040_i2c
     * @{
     */

    /**
     * @brief Config to configure the I2C controller as a master.
     */
    struct MasterConfig {
        SpeedMode speed_mode;
        AddressMode address_mode;
        std::uint32_t sys_clock_hz;
        constexpr MasterConfig(const SpeedMode speed_mode, const AddressMode address_mode, const std::uint32_t sys_clock_hz) :
        speed_mode(speed_mode), address_mode(address_mode), sys_clock_hz(sys_clock_hz) {}
    };

    /**
     * @brief The I2C controller.
     *
     * @tparam base Base address of the I2C controller.
     */
    template <std::uintptr_t base>
    class I2cType {
        static_assert((base == address::i2c0_base) || (base == address::i2c1_base), "Wrong i2c base address!");
    public:
        /**
         * @brief Gets the singleton instance.
         *
         * @return I2cType The singleton instance.
         */
        static I2cType& get_instance();

        /**
         * @brief Initializes the I2C controller.
         *
         * This function deasserts the reset of the I2C controller and allowing access to the register of the I2C
         * controller.
         *
         * @pre None
         *
         * @post The I2C controller is initialized and ready to be configured.
         */
        void init();

        /**
         * @brief Deinitializes the I2C controller.
         *
         * @pre None
         *
         * @post The I2C controller is deinitialized and the registers are inaccessible.
         */
        void deinit();

        template <MasterConfig config>
        status::Status configure();

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
