/**
 * @file xosc.tpp
 * @brief Implementation of the crystal oscillator (XOSC).
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

#ifndef RP2040_SYSTEM_XOSC_T_H
#define RP2040_SYSTEM_XOSC_T_H

#include "include/rp2040/system/xosc.h"

namespace rp2040::system::xosc {
    template<std::uintptr_t xosc_addr>
    XoscType<xosc_addr>& XoscType<xosc_addr>::get_instance() {
        static XoscType<xosc_addr> instance;
        return instance;
    }

    template<std::uintptr_t xosc_addr>
    void XoscType<xosc_addr>::init() {
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::is_enabled))) {
            return;
        }
        using freq_range_bits_t = ctrl_r::ctrl_reg::template Bits<typename ctrl_r::FreqRangeBitType>;
        using freq_range_value_t = ctrl_r::FreqRangeBitType::value;
        freq_range_bits_t::set(freq_range_value_t::range_1_15_mhz);

        using delay_bits_t = startup_r::startup_reg::template Bits<typename startup_r::template DelayBitType<startup_r::default_delay_val>>;
        using delay_value_t = startup_r::template DelayBitType<startup_r::default_delay_val>::value;
        delay_bits_t::set(delay_value_t::val);

        using enable_bits_t = ctrl_r::ctrl_reg::template Bits<typename ctrl_r::EnableBitType>;
        using enable_value_t = ctrl_r::EnableBitType::value;
        enable_bits_t::set(enable_value_t::enable);

        /* Wait until the crystal oscillator is running and stable */
        using enabled_bits_t = status_r::status_reg::template Bits<typename status_r::EnabledBitType>;
        using enabled_value_t = status_r::EnabledBitType::value;
        while (enabled_bits_t::getValue() != static_cast<status_r::status_reg::RegType>(enabled_value_t::enabled)) {}

        this->m_flags.set(static_cast<std::uint32_t>(Flags::is_enabled));
    }

    template<std::uintptr_t xosc_addr>
    void XoscType<xosc_addr>::deinit() {
        using enable_bits_t = ctrl_r::ctrl_reg::template Bits<typename ctrl_r::EnableBitType>;
        using enable_value_t = ctrl_r::EnableBitType::value;
        enable_bits_t::set(enable_value_t::disable);

        this->m_flags.reset(static_cast<std::uint32_t>(Flags::is_enabled));
    }

    template<std::uintptr_t xosc_addr>
    void XoscType<xosc_addr>::pause() {
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::is_paused))) {
            return;
        }
        using dormant_bits_t = dormant_r::dormant_reg::template Bits<typename dormant_r::DormantBitType>;
        using dormant_value_t = dormant_r::DormantBitType::value;
        dormant_bits_t::set(dormant_value_t::pause);

        this->m_flags.set(static_cast<std::uint32_t>(Flags::is_paused));
    }

    template<std::uintptr_t xosc_addr>
    void XoscType<xosc_addr>::wake() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::is_paused))) {
            return;
        }
        using dormant_bits_t = dormant_r::dormant_reg::template Bits<typename dormant_r::DormantBitType>;
        using dormant_value_t = dormant_r::DormantBitType::value;
        dormant_bits_t::set(dormant_value_t::wake);

        this->m_flags.reset(static_cast<std::uint32_t>(Flags::is_paused));
    }

    template <std::uintptr_t xosc_addr>
    std::expected<unsigned int, status::Status> XoscType<xosc_addr>::get_frequency() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::is_enabled))) {
            return std::unexpected(status::Status::ERROR_INIT);
        }
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::is_paused))) {
            return std::unexpected(status::Status::ERROR_RESOURCE);
        }

        return m_frequency_hz;
    }


}

#endif // RP2040_SYSTEM_XOSC_T_H
