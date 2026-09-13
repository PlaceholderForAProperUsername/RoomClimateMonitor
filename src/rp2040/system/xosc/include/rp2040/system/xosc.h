/**
 * @file xosc.h
 * @brief Definition of the crystal oscillator (XOSC) object.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 15.06.2026
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

#ifndef RP2040_SYSTEM_XOSC_H
#define RP2040_SYSTEM_XOSC_H

#include <bitset>
#include <cstdint>
#include <expected>

#include "xosc_def.h"
#include "status.h"

namespace rp2040::system::xosc {
    /**
     * @addtogroup rp2040_xosc
     * @{
     */

    /**
     * @brief Object to control the crystal oscillator (XOSC) as a Singleton.
     *
     * @tparam xosc_addr The address of the XOSC.
     */
    template<std::uintptr_t xosc_addr>
    class XoscType {
        static_assert(xosc_addr == address::xosc, "Template parameter 'xosc_addr' must be a valid xosc address");

    public:
        /**
         * @brief Gets the XOSC instance.
         *
         * @return XOSC& The instance of the XOSC object.
         */
        static XoscType& get_instance();

        /**
         * @brief Initializes the XOSC.
         *
         * @pre None
         *
         * @post The XOSC is enabled.
         */
        void init();

        /**
         * @brief Disables the XOSC.
         *
         * @pre None
         *
         * @post The XOSC is disabled.
         */
        void deinit();

        /**
         * @brief Pauses the XOSC.
         *
         * @pre The PLL should be disabled.
         * @pre An IRQ should be configured.
         *
         * @post The XOSC is paused.
         *
         * @see wake
         */
        void pause();

        /**
         * @brief Wakes up the XOSC.
         *
         * @pre None
         *
         * @post The XOSC is unpaused.
         *
         * @see pause
         */
        void wake();

        /**
         * @brief Gets the frequency of the XOSC.
         *
         * @return The frequency at which the XOSC runs or an error code.
         * @retval unsigned int The frequency of the XOSC in Hz.
         * @retval ERROR_INIT The frequency is not initialized. @see init
         * @retval ERROR_RESOURCE The XOSC is paused and thus unavailable. @see wake
         */
        std::expected<unsigned int, status::Status> get_frequency();

    private:
        using xosc_regs = XoscRegMapType<xosc_addr>; /**< @brief The register map. */
        using ctrl_r = xosc_regs::Ctrl; /**< @brief Alias for the ctrl register. */
        using status_r = xosc_regs::Status; /**< @brief Alias for the status register. */
        using dormant_r = xosc_regs::Dormant; /**< @brief Alias for the dormant register. */
        using startup_r = xosc_regs::Startup; /**< @brief Alias for the startup register. */
        using count_r = xosc_regs::Count; /**< @brief Alias for the count register. */

        XoscType() : m_flags(0) {};


        /**
         * @brief Status flags of the XOSC
         *
         * The value correspond to the bit position in @ref m_flags
         */
        enum class Flags : std::uint32_t {
            is_enabled = 0, /**< @brief Flag to indicate if the XOSC is enabled. @see init @see deinit */
            is_paused, /**< @brief Flag to indicate if the XOSC is paused. @see pause @see wake */
            number_of_flags /**< @brief Helper entry to get the number of flags. */
        };

        std::bitset<static_cast<std::size_t>(Flags::number_of_flags)> m_flags; /**< @brief The status flags of the XOSC. @see Flags */

        static constexpr unsigned int m_frequency_hz = 12'000'000U; /**< @brief The frequency of the RP2040's XOSC. */
    };

    using XOSC = XoscType<address::xosc>; /**< @brief Alias for the XOSC. */

    /** @}*/ // rp2040_xosc
}


#include "../../../xosc.tpp"

#endif //RP2040_SYSTEM_XOSC_H
