/**
 * @file timer.h
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

#ifndef RP2040_HARDWARE_TIMER_H
#define RP2040_HARDWARE_TIMER_H

#include <bitset>
#include <expected>

#include "status.h"

#include "timer_def.h"

namespace rp2040::hardware::timer {
    /**
     * @addtogroup rp2040_timer
     * @{
     */

    /**
     * @brief The timer peripheral.
     *
     * In order to use the timer peripheral, the tick generator must be running. @see Watchdog
     *
     * @tparam base Base address of the timer peripheral.
     */
    template <std::uintptr_t base>
    class TimerType {
        static_assert(base == address::timer, "Invalid base address for TimerType!");
    public:
        /**
         * @brief Getter function to get the Singleton instance.
         *
         * @return TimerType& The singleton instance.
         */
        static TimerType& get_instance();

        /**
         * @brief Initializes the timer module.
         *
         * @pre None
         *
         * @post The timer module is ready to be configured.
         */
        void init();

        /**
         * @brief Deinitializes the timer module.
         *
         * @pre None
         *
         * @post The timer module is disabled.
         */
        void deinit();

        /**
         * @brief Gets the lower 32 bits of the timer value.
         *
         * @return std::expected<std::uint32_t, status::Status> The lower 32 bits of the timer or a status code in case of error.
         * @retval std::uint32_t The lower 32 bits of the timer.
         * @retval status::Status::INIT The timer module was not initialized. @see init
         * @retval status::Status::RESOURCE The tick generator is not running. @see Watchdog
         */
        std::expected<std::uint32_t, status::Status> get_time_32bit();

    private:
        using Registers = TimerRegMap<base>;

        enum class Flags : std::uint8_t {
            initialized = 0U,
            number_of_flags
        };

        std::bitset<std::to_underlying(Flags::number_of_flags)> m_flags;

        TimerType() : m_flags(0) {};
    };

    using Timer = TimerType<address::timer>;

    /**@]*/
}

#include "../../../timer.tpp"

#endif // RP2040_HARDWARE_TIMER_H
