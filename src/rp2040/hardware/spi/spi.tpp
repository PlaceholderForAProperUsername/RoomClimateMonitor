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

        m_flags.set(std::to_underlying(Flags::initialized));
    }

    template <std::uintptr_t base>
    void SpiType<base>::deinit() {
        if constexpr (base == address::spi0) {
            system::resets::Resets::get_instance().disable<system::resets::Subsystems::spi0>();
        } else if constexpr (base == address::spi1) {
            system::resets::Resets::get_instance().disable<system::resets::Subsystems::spi1>();
        }

        m_flags.reset(std::to_underlying(Flags::initialized));
    }

    /** @}*/ // rp2040_spi
}

#endif //RP2040_HARDWARE_SPI_TPP
