/**
 * @file watchdog.tpp
 * @brief The watchdog module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 20.10.2026
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

#ifndef RP2040_HARDWARE_WATCHDOG_TPP
#define RP2040_HARDWARE_WATCHDOG_TPP

#include "rp2040/hardware/watchdog.h"

namespace rp2040::hardware::watchdog {
    /**
     * @addtogroup rp2040_watchdog
     * @{
     */
    template <std::uintptr_t base>
    WatchdogType<base>& WatchdogType<base>::get_instance() {
        static WatchdogType<base> instance;
        return instance;
    }

    template <std::uintptr_t base>
    template <std::uint16_t cycles>
    void WatchdogType<base>::start_tick_generator() {
        if (Registers::Tick::Running::Bits::get_value() == Registers::Tick::Running::Value::enable) {
            Registers::Tick::Enable::Bits::set(Registers::Tick::Enable::Value::disable);
        }
        using CyclesBits = Registers::Tick::template Cycles<cycles>;
        CyclesBits::Bits::set(CyclesBits::Value::val);

        Registers::Tick::Enable::set(Registers::Tick::Enable::Value::enable);
    }

    /**@}*/ //rp2040_watchdog
}

#endif // RP2040_HARDWARE_WATCHDOG_TPP