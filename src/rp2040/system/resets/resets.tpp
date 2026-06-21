/**
 * @file resets.tpp
 * @brief Implementation of the resets controller.
 * @author Thorsten Hoffmeister
 * @version 0.1
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

namespace rp2040::system {
    /**
     * @addtogroup rp2040_resets
     * @{
     */

    template <std::uintptr_t resets_addr>
    Resets_Type<resets_addr>& Resets_Type<resets_addr>::getInstance() {
        static Resets_Type<resets_addr> instance;
        return instance;
    }

    template <std::uintptr_t resets_addr>
    template <SubsystemBits subsystem>
    void Resets_Type<resets_addr>::enable() {
        using T = reset_r::reset_reg::RegType;
        static_assert(static_cast<T>(subsystem) < static_cast<T>(SubsystemBits::NumberOfSubsystems), "Invalid subsystem");

        using reset_bits_t = reset_r::template reset_bits<subsystem>;
        using reset_value_t = reset_r::template ResetBitField<subsystem>::value;

        reset_bits_t::set(reset_value_t::enable);
    }

    template <std::uintptr_t resets_addr>
    template <SubsystemBits subsystem>
    void Resets_Type<resets_addr>::disable() {
        using T = reset_r::reset_reg::RegType;
        static_assert(static_cast<T>(subsystem) < static_cast<T>(SubsystemBits::NumberOfSubsystems), "Invalid subsystem");

        using reset_bits_t = reset_r::template reset_bits<subsystem>;
        using reset_value_t = reset_r::template ResetBitField<subsystem>::value;

        reset_bits_t::set(reset_value_t::disable);
    }

    template <std::uintptr_t resets_addr>
    template <SubsystemBits subsystem>
    void Resets_Type<resets_addr>::reset() {
        using T = reset_r::reset_reg::RegType;
        static_assert(static_cast<T>(subsystem) < static_cast<T>(SubsystemBits::NumberOfSubsystems), "Invalid subsystem");

        disable<subsystem>();

        enable<subsystem>();
    }

    template <std::uintptr_t resets_addr>
    template <SubsystemBits subsystem>
    bool Resets_Type<resets_addr>::isEnabled() {
        using T = reset_done_r::reset_done_reg::RegType;
        static_assert(static_cast<T>(subsystem) < static_cast<T>(SubsystemBits::NumberOfSubsystems), "Invalid subsystem");

        using reset_done_bits_t = reset_done_r::template reset_done_bits<subsystem>;
        using reset_done_value_t = reset_done_r::template ResetDoneBitField<subsystem>::value;

        return reset_done_bits_t::getValue() == static_cast<T>(reset_done_value_t::enabled);
    }

    /** @} */
} // rp2040::system

#endif // RP2040_SYSTEM_RESETS_T_H