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
        ctrl_r::FreqRange::Bits::set(ctrl_r::FreqRange::Value::range_1_15_mhz);

        startup_r::template Delay<>::Bits::set(startup_r::template Delay<>::Value::val);

        ctrl_r::Enable::Bits::set(ctrl_r::Enable::Value::enable);

        /* Wait until the crystal oscillator is running and stable */
        while (status_r::Enabled::Bits::get_value() != static_cast<status_r::status_reg::RegType>(status_r::Enabled::Value::enabled)) {}

        this->m_flags.set(static_cast<std::uint32_t>(Flags::is_enabled));
    }

    template<std::uintptr_t xosc_addr>
    void XoscType<xosc_addr>::deinit() {
        ctrl_r::Enable::Bits::set(ctrl_r::Enable::Value::disable);

        this->m_flags.reset(static_cast<std::uint32_t>(Flags::is_enabled));
    }

    template<std::uintptr_t xosc_addr>
    void XoscType<xosc_addr>::pause() {
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::is_paused))) {
            return;
        }
        dormant_r::DormantWake::Bits::set(ctrl_r::DormantWake::Value::pause);

        this->m_flags.set(static_cast<std::uint32_t>(Flags::is_paused));
    }

    template<std::uintptr_t xosc_addr>
    void XoscType<xosc_addr>::wake() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::is_paused))) {
            return;
        }
        dormant_r::DormantWake::Bits::set(ctrl_r::DormantWake::Value::wake);

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
