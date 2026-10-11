/**
 * @file spi.tpp
 * @brief The SPI module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 06.10.2026
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
#ifndef RP2040_HARDWARE_SPI_TPP
#define RP2040_HARDWARE_SPI_TPP

#include "include/rp2040/hardware/spi.h"
#include "rp2040/system/resets.h"

namespace rp2040::hardware::spi {
    /**
     * @addtogroup rp2040_spi
     * @{
     */

    template <std::uintptr_t base>
    SpiType<base>& SpiType<base>::get_instance() {
        static SpiType<base> instance;
        return instance;
    }

    template <std::uintptr_t base>
    void SpiType<base>::init() {
        if (m_flags.test(std::to_underlying(Flags::initialized))) {
            return;
        }

        if constexpr (base == address::spi0) {
            system::resets::Resets::get_instance().reset<system::resets::Subsystems::spi0>();
        } else if constexpr (base == address::spi1) {
            system::resets::Resets::get_instance().reset<system::resets::Subsystems::spi1>();
        }

        m_flags.reset(); // reset all other flags, in case some are set.
        m_flags.set(std::to_underlying(Flags::initialized));
    }

    template <std::uintptr_t base>
    void SpiType<base>::deinit() {
        if constexpr (base == address::spi0) {
            system::resets::Resets::get_instance().disable<system::resets::Subsystems::spi0>();
        } else if constexpr (base == address::spi1) {
            system::resets::Resets::get_instance().disable<system::resets::Subsystems::spi1>();
        }

        m_flags.reset();
    }

    template <std::uintptr_t base>
    template <MasterConfig config>
    status::Status SpiType<base>::configure() {
        if (!m_flags.test(std::to_underlying(Flags::initialized))) {
            return status::Status::ERROR_INIT;
        }
        static_assert((config.prescale_divisor >= 2) && (config.prescale_divisor % 2 == 0), "Prescale divisor must be a multiple of 2!");

        using ScrBits = Registers::Cr0::template Scr<config.clock_rate>;
        ScrBits::Bits::set(ScrBits::Value::val);

        Registers::Cr0::Frf::Bits::set(config.format);
        if (config.format == FrameFormat::motorola) {
            if constexpr (config.clock_phase) {
                Registers::Cr0::Sph::Bits::set(Registers::Cr0::Sph::Value::high);
            } else {
                Registers::Cr0::Sph::Bits::clear();
            }
            if constexpr (config.clock_polarity) {
                Registers::Cr0::Spo::Bits::set(Registers::Cr0::Spo::Value::high);
            } else {
                Registers::Cr0::Spo::Bits::clear();
            }
        }
        Registers::Cr0::Dss::Bits::set(config.size);

        using CpsrBits = Registers::Cpsr::template ClockPrescale<config.prescale_divisor>;
        CpsrBits::Bits::set(CpsrBits::Value::val);

        Registers::Cr1::Ms::Bits::set(Registers::Cr1::Ms::Value::master);
        Registers::Cr1::Lbm::Bits::clear();
        Registers::Cr1::Sse::Bits::set(Registers::Cr1::Sse::Value::enable);

        m_flags.set(std::to_underlying(Flags::configured));

        return status::Status::OK;
    }

    /** @}*/ // rp2040_spi
}

#endif //RP2040_HARDWARE_SPI_TPP
