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
            // enables the timers when compiled with cmake build type "Debug"
#ifdef DEBUG_WITH_TIMERS
            Registers::DbgPause::Dbg0::Bits::clear();
            Registers::DbgPause::Dbg1::Bits::clear();
#endif
            this->m_flags.set(std::to_underlying(Flags::initialized));
        }
    }

    template <std::uintptr_t base>
    void TimerType<base>::deinit() {
        rp2040::system::resets::Resets::get_instance().disable<rp2040::system::resets::Subsystems::timer>();
        this->m_flags.reset();
    }

    template <std::uintptr_t base>
    std::expected<std::uint32_t, status::Status> TimerType<base>::get_time_32bit() {
        if (!this->m_flags.test(std::to_underlying(Flags::initialized))) {
            return std::unexpected(status::Status::ERROR_INIT);
        }
        if (!rp2040::hardware::watchdog::Watchdog::get_instance().is_tick_generator_running()) {
            return std::unexpected(status::Status::ERROR_RESOURCE);
        }
        return Registers::TimeRawL::template Timer<>::Bits::get_value();
    }

    /**
     * @internal
     * The function only supports wait time of up to 4'000'000'000 microseconds, as values close to the maximum value may
     * not be counted correctly by the while loop.
     */
    template<std::uintptr_t base>
    status::Status TimerType<base>::busy_wait_us_32(std::uint32_t delay_us) {
        if (delay_us > 4'000'000'000 ) {
            return status::Status::ERROR_PARAM;
        }
        // get_time_32bit is called in this step for error checkings. The error checkings are not needed in the inner while loop
        // and only waste time
        if (auto start = this->get_time_32bit(); start.has_value()) {
            while ((Registers::TimeRawL::template Timer<>::Bits::get_value() - start.value()) < delay_us) {
                asm volatile ("nop");
            }
        }
        else {
            return start.error();
        }

        return status::Status::OK;
    }

    template<std::uintptr_t base>
    status::Status TimerType<base>::busy_wait_ms_32(std::uint32_t delay_ms) {
        if (delay_ms > 4'000'000 ) {
            return status::Status::ERROR_PARAM;
        }
        constexpr std::uint32_t us_to_ms_factor {1000};
        std::uint32_t delay_us {delay_ms * us_to_ms_factor};
        return this->busy_wait_us_32(delay_us);
    }
    /**@]*/
}

#endif // RP2040_HARDWARE_TIMER_TPP