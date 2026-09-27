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
#include <variant>

#include "clocks_def.h"

namespace rp2040::system::clocks {
    /**
     * @addtogroup rp2040_clocks
     * @{
     */

    /**
     * @brief Clock configuration.
     *
     * There are three types of clocks:
     * - Clocks without a div register
     * - Clocks with a div register with only the integer component
     * - Clocks with a div register with integer and fractional component
     *
     * The clock type is determined by ClockLayout
     *
     * @tparam ClockLayout Layout of the clock
     */
    template <ClockLayoutConcept ClockLayout>
    struct ClockConfType {
        ClockLayout::Src src; /**< @brief The source of the clock. */
        std::uint32_t frequency_hz; /**< @brief The frequency of the clock source. */
        [[no_unique_address]] std::conditional_t<(ClockLayout::Div::int_mask > 0), std::uint32_t, std::monostate> div_int; /**< @brief The integer component of the clock divisor. */
        [[no_unique_address]] std::conditional_t<(ClockLayout::Div::frac_mask > 0), std::uint8_t, std::monostate> div_frac; /**< @brief The fractional component of the clock divider. */


        /**
         * @brief Creates a compile time instance to hold the clock configuration data.
         *
         * @param[in] src The clock source.
         * @param[in] frequency_hz The frequency of the clock source.
         */
        template <ClockLayoutConcept Layout = ClockLayout, typename = std::enable_if_t<(Layout::Div::int_mask == 0) && (Layout::Div::frac_mask == 0)>>
        constexpr ClockConfType(const ClockLayout::Src src, const std::uint32_t frequency_hz) :
        src(src), frequency_hz(frequency_hz) {}

        /**
         * @brief Creates a compile time instance to hold the clock configuration data.
         *
         * @param[in] src The clock source.
         * @param[in] frequency_hz The frequency of the clock source.
         * @param[in] div_int The integer part of the clock divisor.
         */
        template <ClockLayoutConcept Layout = ClockLayout, typename = std::enable_if_t<(Layout::Div::int_mask > 0) && (Layout::Div::frac_mask == 0)>>
        constexpr ClockConfType(const ClockLayout::Src src, const std::uint32_t frequency_hz, const std::uint32_t div_int = 0x01U) :
        src(src), frequency_hz(frequency_hz), div_int(div_int) {}

        /**
         * @brief Creates a compile time instance to hold the clock configuration data.
         *
         * @param[in] src The clock source.
         * @param[in] frequency_hz The frequency of the clock source.
         * @param[in] div_int The integer part of the clock divisor.
         * @param[in] div_frac The fractional part of the clock divisor.
         */
        template <ClockLayoutConcept Layout = ClockLayout, typename = std::enable_if_t<(Layout::Div::int_mask > 0) && (Layout::Div::frac_mask > 0)>>
        constexpr ClockConfType(const ClockLayout::Src src, const std::uint32_t frequency_hz, const std::uint32_t div_int = 0x01U, const std::uint8_t div_frac = 0x00U) :
        src(src), frequency_hz(frequency_hz), div_int(div_int), div_frac(div_frac) {}
    };


    /**
     * @brief Provides general functionality common to the clocks of the RP2040.
     *
     */
    template <ClockLayoutConcept ClockLayout>
    class ClockBaseType {
        static_assert((ClockLayout::addr == address::base::gpout0) || (ClockLayout::addr == address::base::gpout1) ||
            (ClockLayout::addr == address::base::gpout2) || (ClockLayout::addr == address::base::gpout3) ||
            (ClockLayout::addr == address::base::ref) || (ClockLayout::addr == address::base::sys) ||
            (ClockLayout::addr == address::base::peri) || (ClockLayout::addr == address::base::usb) ||
            (ClockLayout::addr == address::base::adc) || (ClockLayout::addr == address::base::rtc),
            "Invalid base address for clock regmap!");

    public:
        /**
         * @brief Getter function to get the singleton instance.
         *
         * @return The singleton instance.
         */
        static ClockBaseType& get_instance();

        /**
         * @brief Configures the clock.
         *
         * @tparam conf The configuration of the clock.
         *
         * @pre The source must be configured and running.
         * @pre When selecting an auxiliary source for a clock with glitchless src bits in the ctrl register, the clock
         * with the value 0x00U must be enabled and running, because this function will temporarily switch to that source.
         *
         * @post The clock is now configured and running.
         */
        template <ClockConfType<ClockLayout> conf, ClockLayoutConcept layout = ClockLayout>
        std::enable_if_t<(layout::Ctrl::src_max_val > 0), void> configure();

        /**
         * @brief Configures the clock.
         *
         * @tparam conf The configuration of the clock.
         *
         * @pre The source must be configured and running.
         *
         * @post The clock is now configured and running.
         */
        template <ClockConfType<ClockLayout> conf, ClockLayoutConcept layout = ClockLayout>
        std::enable_if_t<(layout::Ctrl::src_max_val == 0), void> configure();

        /**
         * @brief Returns the frequency at which the clock is running.
         *
         * @return std::uint32_t The frequency of the clock in hertz.
         */
        std::uint32_t get_frequency_hz() const;

    private:
        using ClockRegister = ClockRegMap<ClockLayout>;
        using ctrl_r = ClockRegister::Ctrl;
        using div_r = ClockRegister::Div;
        using selected_r = ClockRegister::Selected;

        std::uint32_t m_frequency_hz;

        ClockBaseType() : m_frequency_hz(0) {};

        /**
         * @brief Helper function to set the clock divisor.
         *
         * Setting the clock divisor bits requires access to bits not every clock has implemented. This can cause the
         * compilation to fail, even if the required bits are gated by 'if constexpr' and technically shouldn't be in the
         * code. Therefore this helper function was added.
         *
         * @tparam integer The integer part of the clock divisor.
         * @tparam frac The fractional part of the clock divisor.
         * @tparam layout The layout of the clock, required as direct template parameter for std::enable_if_t.
         */
        template <std::uint32_t integer, std::uint32_t frac , ClockLayoutConcept layout = ClockLayout>
        std::enable_if_t<(layout::Div::int_mask > 0) && (layout::Div::frac_mask > 0), void>
        _set_divisor ();

        /**
         * @brief Helper function to set the clock divisor.
         *
         * Setting the clock divisor bits requires access to bits not every clock has implemented. This can cause the
         * compilation to fail, even if the required bits are gated by 'if constexpr' and technically shouldn't be in the
         * code. Therefore this helper function was added.
         *
         * @tparam integer The integer part of the clock divisor.
         * @tparam layout The layout of the clock, required as direct template parameter for std::enable_if_t.
         */
        template <std::uint32_t integer, ClockLayoutConcept layout = ClockLayout>
        std::enable_if_t<(layout::Div::int_mask > 0) && (layout::Div::frac_mask == 0), void>
        _set_divisor ();
    };

    /**
     * @brief The objects to configure a clock.
     *
     * @tparam ClockLayout The layout describing the clock.
     */
    template <ClockLayoutConcept ClockLayout>
    struct ClockHandle {
        using Clock = ClockBaseType<ClockLayout>; /**< @brief Object to control a clock. */
        using Config = ClockConfType<ClockLayout>; /**< @brief Stores the configuration of a clock. */
        using Src = ClockLayout::Src; /**< @brief Sources of the clock. */
    };

    using Ref = ClockHandle<layout::Ref>;
    using Sys = ClockHandle<layout::Sys>;
    using Peri = ClockHandle<layout::Peri>;

    /** @}*/ // rp2040_clocks
} // rp2040::system::clocks


#include "../../../clocks.tpp"

#endif // RP2040_SYSTEM_CLOCKS_H
