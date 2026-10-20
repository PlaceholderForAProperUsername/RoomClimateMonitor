/**
 * @file watchdog_def.h
 * @brief The definitions of the watchdog module of the RP2040.
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

#ifndef RP2040_HARDWARE_WATCHDOG_DEF_H
#define RP2040_HARDWARE_WATCHDOG_DEF_H

#include <cstdint>

namespace rp2040::hardware::watchdog {
    /**
     * @addtogroup rp2040_watchdog
     * @{
     */

    /**
     * @brief Addresses of the watchdog module.
     */
    namespace address {
        constexpr std::uintptr_t watchdog {0x40058000U}; /**< @brief Base address of the watchdog module. */
    }

    template <std::uintptr_t base>
    struct RegMap {
        static_assert(base == address::watchdog, "Invalid base address for watchdog RegMap!");

        /**
         * @brief Register offsets of the watchdog module.
         */
        struct Offset {
            static constexpr std::uint32_t ctrl {0x00U}; /**< @brief Offset of the control register. */
            static constexpr std::uint32_t load {0x04U}; /**< @brief Offset of the load register. */
            static constexpr std::uint32_t reason {0x08U}; /**< @brief Offset of the reason register. */
            static constexpr std::uint32_t scratch0 {0x0CU}; /**< @brief Offset of the scratch0 register. */
            static constexpr std::uint32_t scratch1 {0x10U}; /**< @brief Offset of the scratch1 register. */
            static constexpr std::uint32_t scratch2 {0x14U}; /**< @brief Offset of the scratch2 register. */
            static constexpr std::uint32_t scratch3 {0x18U}; /**< @brief Offset of the scratch3 register. */
            static constexpr std::uint32_t scratch4 {0x1CU}; /**< @brief Offset of the scratch4 register. */
            static constexpr std::uint32_t scratch5 {0x20U}; /**< @brief Offset of the scratch5 register. */
            static constexpr std::uint32_t scratch6 {0x24U}; /**< @brief Offset of the scratch6 register. */
            static constexpr std::uint32_t scratch7 {0x28U}; /**< @brief Offset of the scratch7 register. */
            static constexpr std::uint32_t tick {0x2CU}; /**< @brief Offset of the tick register. */
        };
    };

    /**@}*/ //rp2040_watchdog
}

#endif // RP2040_HARDWARE_WATCHDOG_DEF_H
