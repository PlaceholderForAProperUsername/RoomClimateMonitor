/**
*  @file resets.h
 * @brief The resets controller to enable or disable subsystems.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 19.06.2026
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

#ifndef RP2040_SYSTEM_RESETS_H
#define RP2040_SYSTEM_RESETS_H

#include <cstdint>

#include "resets_def.h"

namespace rp2040::system {
    /**
    * @addtogroup rp2040_resets
    * @{
    */

    /**
     * @brief The resets controller to enable and disable subsystems.
     *
     * @tparam resets_addr The address of the resets controller.
     */
    template<std::uintptr_t resets_addr>
    class Resets_Type {
        static_assert(resets_addr == resets_base, "The resets address must be a valid address");

        using resets_reg = Resets_RegMapType<resets_addr>; /**< @brief The registers of the reset controller */
        using reset_r = resets_reg::reset_r; /**< @brief The reset register. */
        using wdsel_r = resets_reg::wdsel_r; /**< @brief The watchdog select regiser. */
        using reset_done_r = resets_reg::reset_done_r; /**< @brief The reset done register. */

    public:
        /**
         * @brief Gets the Resets object.
         *
         * @return Resets_Type& The resets instance.
         */
        static Resets_Type& getInstance();

        /**
         * @brief Enables a subsystem.
         *
         * @tparam subsystem The subsystem to be enabled.
         */
        template <SubsystemBits subsystem>
        void enable();

    private:
        Resets_Type() = default;
    };

    using resets = Resets_Type<resets_base>;

    /** @} */ // rp2040_resets
} // rp2040::system

#include "../../../resets.tpp"

#endif // RP2040_SYSTEM_RESETS_H
