/**
 * @file clocks.h
 * @brief The Clocks module to use the clocks of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 03.07.2026
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

#ifndef RP2040_SYSTEM_CLOCKS_H
#define RP2040_SYSTEM_CLOCKS_H

#include <cstdint>

#include "clocks_def.h"

namespace rp2040::system::clocks {
    /**
     * @addtogroup rp2040_clocks
     * @{
     */

    /**
     * @brief Provides general functionality common to the clocks of the RP2040.
     *
     * @tparam ClockDefType A clock type.
     */
    template <typename ClockDefType>
    requires IsClockType<ClockDefType>
    class ClockBaseType {
        static constexpr decltype(auto) addr = ClockDefType::base_addr;

        static_assert((addr == ref_base) || (addr == sys_base) || (addr == peri_base), "Invalid clock address.");

        using ctrl_r = ClockDefType::ctrl;
        using div_r = decltype([] {
            if constexpr (requires {typename ClockDefType::div;}) {
                return std::type_identity<typename ClockDefType::div>();
            } else {
                return std::type_identity<void>();
            }
        }())::type;
        using selected_r = decltype([] {
            if constexpr (requires {typename ClockDefType::selected;}) {
                return std::type_identity<typename ClockDefType::selected>();
            } else {
                return std::type_identity<void>();
            }
        }())::type;

    public:
        /**
         * @brief Getter function to get the singleton instance.
         *
         * @return The singleton instance.
         */
        static ClockBaseType<ClockDefType>& getInstance();

        template <typename ClockDefType::ClockSrc src>
        void setClkSrc();

    private:
        ClockBaseType() = default;

    };
    
    using RefClock = ClockBaseType<RefClock_DefType>; /**< @brief The reference clock. */
    /** @}*/ // rp2040_clocks
} // rp2040::system::clocks


#include "../../../clocks.tpp"

#endif // RP2040_SYSTEM_CLOCKS_H
