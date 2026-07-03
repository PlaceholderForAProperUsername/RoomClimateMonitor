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

namespace rp2040::system {
    template<std::uintptr_t xosc_addr>
    XOSC_Type<xosc_addr>& XOSC_Type<xosc_addr>::getInstance() {
        static XOSC_Type<xosc_addr> inst;
        return inst;
    }

    template<std::uintptr_t xosc_addr>
    void XOSC_Type<xosc_addr>::init() {
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::IsEnabled))) {
            return;
        }
        ctrl_r::freq_range_bits::set(ctrl_r::FreqRangeBitField::value::range_1_15_mhz);

        startup_r::template delay_bits<>::set(startup_r::template DelayBitField<>::value::val);

        ctrl_r::enable_bits::set(ctrl_r::EnableBitField::value::enable);

        /* Wait until the crystal oscillator is running and stable */
        while (!status_r::enabled_bits::getBits()) {}

        this->m_flags.set(static_cast<std::uint32_t>(Flags::IsEnabled));
    }

    template<std::uintptr_t xosc_addr>
    void XOSC_Type<xosc_addr>::deinit() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::IsEnabled))) {
            return;
        }

        ctrl_r::enable_bits::set(ctrl_r::EnableBitField::value::disable);

        this->m_flags.reset(static_cast<std::uint32_t>(Flags::IsEnabled));
    }

    template<std::uintptr_t xosc_addr>
    void XOSC_Type<xosc_addr>::pause() {
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::IsPaused))) {
            return;
        }
        dormant_r::dormant_bits::set(dormant_r::dormant_bits::value::pause);

        this->m_flags.set(static_cast<std::uint32_t>(Flags::IsPaused));
    }

    template<std::uintptr_t xosc_addr>
    void XOSC_Type<xosc_addr>::wake() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::IsPaused))) {
            return;
        }
        dormant_r::dormant_bits::set(dormant_r::dormant_bits::value::pause);

        this->m_flags.reset(static_cast<std::uint32_t>(Flags::IsPaused));
    }

    template <std::uintptr_t xosc_addr>
    std::expected<unsigned int, utils::status::Status> XOSC_Type<xosc_addr>::getFrequency() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::IsEnabled))) {
            return std::unexpected(utils::status::Status::ERROR_INIT);
        }
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::isPaused))) {
            return std::unexpected(utils::status::Status::ERROR_RESOURCE);
        }

        return m_frequency_hz;
    }


}

#endif // RP2040_SYSTEM_XOSC_T_H
