/**
 * @file pll.h
 * @brief The interface to configure the pll.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 24.06.2026
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

#ifndef RP2040_SYSTEM_PLL_H
#define RP2040_SYSTEM_PLL_H

#include <bitset>
#include <expected>

#include "utils/status.h"
#include "pll_def.h"


namespace rp2040::system::pll {
    /**
     * @addtogroup rp2040_pll
     * @{
     */

    /**
     * @brief Holds the configuration parameter for the PLL.
     *
     * The default parameter are for PLL_SYS to achieve a system frequency of 125MHz with the XOSC (12MHz) set as the
     * reference clock.
     */
     struct PLL_ConfigType {
         std::uint8_t refdiv; /**< @brief The reference clock divider value. */
         std::uint8_t postdiv1; /**< @brief The post divider 1 value. */
         std::uint8_t postdiv2; /**< @brief The post divider 2 value. */
         std::uint16_t fbdiv; /**< @brief The feedback divider value. */

         /**
          * @brief Constructor of the PLL_ConfigType object to make this object usable at compile time.
          *
          * @param refdiv The value of the reference clock divisor.
          * @param postdiv1 The post divider 1 value.
          * @param postdiv2 The post divider 2 value.
          * @param fbdiv The feedback divider value.
          */
         constexpr explicit PLL_ConfigType(std::uint8_t refdiv = 1U, std::uint8_t postdiv1 = 6U,
                                           std::uint8_t postdiv2 = 2U, std::uint16_t fbdiv = 125U) :
         refdiv(refdiv), postdiv1(postdiv1), postdiv2(postdiv2), fbdiv(fbdiv) {}

    };

    /**
     * @brief The pll object.
     *
     * @tparam pll_addr The address of the pll.
     */
    template <std::uintptr_t pll_addr>
    class PLL_Type {
        static_assert((pll_addr == pll_sys_base) || (pll_addr == pll_usb_base), "Invalid pll address.");

        using pll_regs = PLL_RegMapType<pll_addr>; /**< @brief The register map of the pll. */
        using cs_r = pll_regs::cs; /**< @brief The control and status register. */
        using pwr_r = pll_regs::pwr; /**< @brief The power modes register. */
        using fbdiv_int_r = pll_regs::fbdiv_int; /**< @brief The feedback divisor register. */
        using prim_r = pll_regs::prim;  /**< @brief The register to control the post dividers. */

    public:
        /**
         * @brief Getter function to get the Instance of the singleton.
         *
         * @return PLL_Type<pll_addr>& The instance of the singleton.
         */
        static PLL_Type<pll_addr>& getInstance();

        /**
         * @brief Enables the PLL, if the PLL is not already enabled.
         *
         * This function puts the PLL only in an enabled state, allowing the PLL to be configured and started. This
         * function does not start the PLL.
         *
         * @pre None
         *
         * @post The PLL is enabled and can be configured.
         */
        void init();

        /**
         * @brief Disables the PLL, if the PLL is not already disabled.
         *
         * @pre None
         *
         * @post The PLL is disabled and its registers are locked.
         */
        void deinit();

        /**
         * @brief Sets the frequency of the PLL.
         *
         * @tparam config The Parameter to configure the PLL. @see PLL_ConfigType
         * @tparam refFreq_Hz The frequency of the reference clock. The default is the frequency of the XOSC @see XOSC_Type
         * @return The achieved frequency in Hz or an error code.
         * @retval ERROR_INIT The PLL is not initialized. @see init
         *
         * @pre The PLL must be initialized. @see init
         *
         * @post The PLL runs at the configured frequency.
         */
        template <PLL_ConfigType config, std::uint32_t refFreq_Hz = 12'000'000U>
        std::expected<std::uint32_t, utils::status::Status> setFrequency();


    private:
        PLL_Type() : m_flags(0), m_frequency(0) {}

        enum class Flags : std::uint32_t {
            isInitialized = 0U,
            NumberOfFlags
        };

        std::bitset<static_cast<std::uint32_t>(Flags::NumberOfFlags)> m_flags;

        unsigned int m_frequency;
    };

    using PLL_Sys = PLL_Type<pll_sys_base>; /**< @brief The System PLL. */
    using PLL_USB = PLL_Type<pll_usb_base>; /**< @brief The USB PLL. */
    /** @} */ // rp2040_pll
} // rp2040::system


#include "../../../pll.tpp"

#endif // RP2040_SYSTEM_PLL_H
