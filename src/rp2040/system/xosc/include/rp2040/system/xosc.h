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

#include "xosc_def.h"

namespace rp2040::system {
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
    class XOSC_Type {
        static_assert(xosc_addr == xosc_base, "Template parameter 'xosc_addr' must be a valid xosc address");

        using xosc_regs = XOSC_RegMapType<xosc_addr>; /**< @brief The register map. */
        using ctrl_r = xosc_regs::ctrl; /**< @brief Alias for the ctrl register. */
        using status_r = xosc_regs::status; /**< @brief Alias for the status register. */
        using dormant_r = xosc_regs::dormant; /**< @brief Alias for the dormant register. */
        using startup_r = xosc_regs::startup; /**< @brief Alias for the startup register. */
        using count_r = xosc_regs::count; /**< @brief Alias for the count register. */

    public:
        /**
         * @brief Gets the XOSC instance.
         *
         * @return XOSC& The instance of the XOSC object.
         */
        static XOSC_Type& getInstance();

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

    private:
        XOSC_Type() : m_flags(0) {};


        /**
         * @brief Status flags of the XOSC
         *
         * The value correspond to the bit position in @ref m_flags
         */
        enum class Flags : std::uint32_t {
            IsEnabled = 0, /**< @brief Flag to indicate if the XOSC is enabled. @see init @see deinit */
            IsPaused, /**< @brief Flag to indicate if the XOSC is paused. @see pause @see wake */
            NumberOfFlags /**< @brief Helper entry to get the number of flags. */
        };

        std::bitset<static_cast<std::size_t>(Flags::NumberOfFlags)> m_flags; /**< @brief The status flags of the XOSC. @see Flags */
    };

    using XOSC = XOSC_Type<xosc_base>; /**< @brief Alias for the XOSC. */

    /** @}*/ // rp2040_xosc
}


#include "../../../xosc.tpp"

#endif //RP2040_SYSTEM_XOSC_H
