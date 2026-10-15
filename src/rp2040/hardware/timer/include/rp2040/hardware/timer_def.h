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


    /**
     * @brief Registers of the timer module.
     *
     * @tparam addr Base address of the timer module.
     */
    template <std::uintptr_t addr>
    struct TimerRegMap {
        static_assert(addr == address::timer, "Invalid base address for TimerRegMap!");

        /**
         * @brief Offsets of the registers from the base address.
         */
        struct Offset {
            static constexpr std::uint32_t time_hw = 0x00U; /**< @brief Offset of the write high bits register. */
            static constexpr std::uint32_t time_lw = 0x04U; /**< @brief Offset of the write low bits register. */
            static constexpr std::uint32_t time_hr = 0x08U; /**< @brief Offset of the read high bits register. */
            static constexpr std::uint32_t time_lr = 0x0CU; /**< @brief Offset of the read low bits register. */
            static constexpr std::uint32_t alarm0 = 0x10U; /**< @brief Offset of the alarm 0 register. */
            static constexpr std::uint32_t alarm1 = 0x14U; /**< @brief Offset of the alarm 1 register. */
            static constexpr std::uint32_t alarm2 = 0x18U; /**< @brief Offset of the alarm 2 register. */
            static constexpr std::uint32_t alarm3 = 0x1CU; /**< @brief Offset of the alarm 3 register. */
            static constexpr std::uint32_t armed = 0x20U; /**< @brief Offset of the armed register. */
            static constexpr std::uint32_t timer_rawh = 0x24U; /**< @brief Offset of the raw read high bits register. */
            static constexpr std::uint32_t timer_rawl = 0x28U; /**< @brief Offset of the raw read low bits register. */
            static constexpr std::uint32_t dbg_pause = 0x2CU; /**< @brief Offset of the debug pause register. */
            static constexpr std::uint32_t pause = 0x30U; /**< @brief Offset of the pause register. */
            static constexpr std::uint32_t intr = 0x34U; /**< @brief Offset of the raw interrupts register. */
            static constexpr std::uint32_t inte = 0x38U; /**< @brief Offset of the interrupt enable register. */
            static constexpr std::uint32_t intf = 0x3CU; /**< @biref Offset of the interrupt force register. */
            static constexpr std::uint32_t ints = 0x40U; /**< @brief Offset of the interrupt status register. */
        };
    };

    /**@]*/
}

#endif // RP204_HARDWARE_TIMER_DEF_H
