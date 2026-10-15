/**
 * @file timer.h
 * @brief The definitions of the Timer module of the RP2040.
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
#ifndef RP204_HARDWARE_TIMER_DEF_H
#define RP204_HARDWARE_TIMER_DEF_H

#include <cstdint>

namespace rp2040::hardware::timer {
    /**
     * @addtogroup rp2040_timer
     * @{
     */

    /**
     * @brief The addresses of the timer module.
     */
    namespace address {
        static constexpr std::uintptr_t timer =  0x40054000U; /**< @brief Base address of the timer. */
    }

    /**@]*/
}

#endif // RP204_HARDWARE_TIMER_DEF_H
