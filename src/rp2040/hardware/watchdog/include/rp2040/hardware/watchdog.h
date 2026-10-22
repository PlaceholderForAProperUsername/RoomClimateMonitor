/**
 * @file watchdog.h
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

#ifndef RP2040_HARDWARE_WATCHDOG_H
#define RP2040_HARDWARE_WATCHDOG_H

#include "watchdog_def.h"

namespace rp2040::hardware::watchdog {
    /**
     * @addtogroup rp2040_watchdog
     * @{
     */

    /**
     * @brief The watchdog controller.
     */
    template <std::uintptr_t base>
    class WatchdogType {
        static_assert(base == address::watchdog, "Invalid watchdog base address!");

    public:
        /**
         * @brief Getter function to get the singleton instance.
         *
         * @return WatchdogType& The watchdog controller instance.
         */
        static WatchdogType& get_instance();

    private:
        WatchdogType() = default;
    };

    /**@}*/ //rp2040_watchdog
}

#include "../../../watchdog.tpp"

#endif // RP2040_HARDWARE_WATCHDOG_H
