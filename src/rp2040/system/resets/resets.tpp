/**
 * @file resets.tpp
 * @brief Implementation of the resets controller.
 * @author Thorsten Hoffmeister
 * @version 1.0
 * @date 21.06.2026
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


#ifndef RP2040_SYSTEM_RESETS_T_H
#define RP2040_SYSTEM_RESETS_T_H

#include "include/rp2040/system/resets.h"

namespace rp2040::system::resets {
    /**
     * @addtogroup rp2040_resets
     * @{
     */

    template <std::uintptr_t resets_addr>
    ResetsType<resets_addr>& ResetsType<resets_addr>::get_instance() {
        static ResetsType<resets_addr> instance;
        return instance;
    }

    template <std::uintptr_t resets_addr>
    template <Subsystems subsystem>
    void ResetsType<resets_addr>::enable() {
        static_assert(subsystem < Subsystems::NumberOfSubsystems, "Invalid template parameter subsystem in ResetsType::enable()!");

        using reset_bits_t = reset_r::reset_reg::template Bits<typename reset_r::template ResetBitType<subsystem>>;
        using reset_value_t = reset_r::template ResetBitType<subsystem>::value;

        reset_bits_t::set(reset_value_t::enable);
    }

    template <std::uintptr_t resets_addr>
    template <Subsystems subsystem>
    void ResetsType<resets_addr>::disable() {
        static_assert(subsystem < Subsystems::NumberOfSubsystems, "Invalid template parameter subsystem in ResetsType::disable()!");

        using reset_bits_t = reset_r::reset_reg::template Bits<typename reset_r::template ResetBitType<subsystem>>;
        using reset_value_t = reset_r::template ResetBitType<subsystem>::value;

        reset_bits_t::set(reset_value_t::disable);
    }

    template <std::uintptr_t resets_addr>
    template <Subsystems subsystem>
    void ResetsType<resets_addr>::reset() {
        static_assert(subsystem < Subsystems::NumberOfSubsystems, "Invalid template parameter subsystem in ResetsType::reset()!");

        disable<subsystem>();

        enable<subsystem>();
    }

    template <std::uintptr_t resets_addr>
    template <Subsystems subsystem>
    bool ResetsType<resets_addr>::is_enabled() {
        static_assert(subsystem < Subsystems::NumberOfSubsystems, "Invalid template parameter subsystem in ResetsType::is_enabled()!");

        using reset_done_bits_t = reset_done_r::reset_done_reg::template Bits<typename reset_done_r::template ResetDoneBitType<subsystem>>;
        using reset_done_value_t = reset_done_r::template ResetDoneBitType<subsystem>::value;
        using T = reset_done_r::reset_done_reg::RegType;

        return reset_done_bits_t::getValue() == static_cast<T>(reset_done_value_t::enabled);
    }

    /** @} */
} // rp2040::system

#endif // RP2040_SYSTEM_RESETS_T_H