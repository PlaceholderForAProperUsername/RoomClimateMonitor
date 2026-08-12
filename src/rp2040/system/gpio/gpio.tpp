/**
 * @file gpio.tpp
 * @brief The GPIO module to use the gpios of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 26.07.2026
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


#ifndef RP2040_SYSTEM_GPIO_TPP
#define RP2040_SYSTEM_GPIO_TPP

#include "rp2040/system/gpio.h"
#include "rp2040/system/resets.h"

namespace rp2040::system::gpio {
    /**
     * @addtogroup rp2040_gpio
     * @{
     */

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    GpioType<ioBankBase, padsBankBase, sioBase>& GpioType<ioBankBase, padsBankBase, sioBase>::getInstance() {
        static GpioType<ioBankBase, padsBankBase, sioBase> instance;
        return instance;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    void GpioType<ioBankBase, padsBankBase, sioBase>::init() {
        if (m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return;
        }

        Resets::getInstance().reset<SubsystemBits::IO_BANK0>();
        Resets::getInstance().reset<SubsystemBits::PADS_BANK0>();

        m_flags.set(static_cast<std::uint32_t>(Flags::Initialized));
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    void GpioType<ioBankBase, padsBankBase, sioBase>::deinit() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return;
        }

        Resets::getInstance().disable<SubsystemBits::IO_BANK0>();
        Resets::getInstance().disable<SubsystemBits::PADS_BANK0>();

        m_flags.reset(static_cast<std::uint32_t>(Flags::Initialized));
    }

    /** @} */ // rp2040_gpio
} // rp2040::system::gpio

#endif //RP2040_SYSTEM_GPIO_TPP
