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
     * @brief Contains basic clock configuration parameters.
     *
     * @tparam ClockDefType The clock to be configured.
     */
    template <typename ClockDefType>
    requires IsClockType<ClockDefType>
    struct ClockConfType {
        ClockDefType::ClockSrc src; /**< @brief The source of the clock. */
        std::uint32_t frequency_hz; /**< @brief The frequency of the clock source. */

        constexpr ClockConfType(ClockDefType::ClockSrc src, std::uint32_t frequency_hz) : src(src), frequency_hz(frequency_hz) {}
    };

    /**
     * @brief Contains clock configuration parameters for clocks with a div register with additional fractional bits.
     *
     * @tparam ClockDefType The clock to be configured.
     */
    template <typename ClockDefType>
    requires IsClockType<ClockDefType> && HasDivWithFracRegister<ClockDefType>
    struct ClockConfType<ClockDefType> {
        ClockDefType::ClockSrc src; /**< @brief The source of the clock. */
        std::uint32_t frequency_hz; /**< @brief The frequency of the clock source. */
        std::uint32_t div_int; /**< @brief The integer component of the clock divisor. */
        std::uint32_t div_frac; /**< @brief The fractional component of the clock divider. */

        constexpr ClockConfType(ClockDefType::ClockSrc src, std::uint32_t frequency_hz, std::uint32_t div_int = 0x01U, std::uint32_t div_frac = 0x00U) :
        src(src), frequency_hz(frequency_hz), div_int(div_int), div_frac(div_frac) {}
    };

    /**
     * @brief Contains clock configuration parameters for clocks with a div register.
     *
     * @tparam ClockDefType The clock to be configured.
     */
    template <typename ClockDefType>
    requires IsClockType<ClockDefType> && HasDivRegister<ClockDefType>
    struct ClockConfType<ClockDefType> {
        ClockDefType::ClockSrc src; /**< @brief The source of the clock. */
        std::uint32_t frequency_hz; /**< @brief The frequency of the clock source. */
        std::uint32_t div_int; /**< @brief The integer component of the clock divisor. */

        constexpr ClockConfType(ClockDefType::ClockSrc src, std::uint32_t frequency_hz, std::uint32_t div_int = 0x01U) :
        src(src), frequency_hz(frequency_hz), div_int(div_int) {}
    };

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
        ClockBaseType() : m_frequency_hz(0) {};

        std::uint32_t m_frequency_hz;

    };
    
    using RefClock = ClockBaseType<RefClock_DefType>; /**< @brief The reference clock. */
    using SysClock = ClockBaseType<SysClock_DefType>; /**< @brief The system clock. */

    using RefClockConfig = ClockConfType<RefClock_DefType>;
    using SysClockConfig = ClockConfType<SysClock_DefType>;
    /** @}*/ // rp2040_clocks
} // rp2040::system::clocks


#include "../../../clocks.tpp"

#endif // RP2040_SYSTEM_CLOCKS_H
