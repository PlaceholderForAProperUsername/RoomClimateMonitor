/**
 * @file timer.tpp
 * @brief The Timer module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 15.10.2026
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

#ifndef RP2040_HARDWARE_TIMER_TPP
#define RP2040_HARDWARE_TIMER_TPP

#include <ranges>

#include "rp2040/hardware/timer.h"
#include "rp2040/hardware/watchdog.h"
#include "rp2040/system/resets.h"

namespace rp2040::hardware::timer {
    /**
     * @addtogroup rp2040_timer
     * @{
     */

    template <std::uintptr_t base>
    TimerType<base>& TimerType<base>::get_instance() {
        static TimerType<base> instance;
        return instance;
    }

    template <std::uintptr_t base>
    void TimerType<base>::init() {
        if (!this->m_flags.test(std::to_underlying(Flags::initialized))) {
            this->m_flags.reset();
            rp2040::system::resets::Resets::get_instance().reset<rp2040::system::resets::Subsystems::timer>();
            this->m_flags.set(std::to_underlying(Flags::initialized));
        }
    }

    /**@]*/
}

#endif // RP2040_HARDWARE_TIMER_TPP