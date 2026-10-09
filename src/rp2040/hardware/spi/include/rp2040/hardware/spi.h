/**
 * @file spi.h
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


#ifndef RP2040_HARDWARE_SPI_H
#define RP2040_HARDWARE_SPI_H

#include <bitset>

#include "spi_def.h"

namespace rp2040::hardware::spi {
    /**
     * @addtogroup rp2040_spi
     * @{
     */

    /**
     * @brief The SPI controller
     *
     * @tparam base Base address of the SPI controller.
     */
    template <std::uintptr_t base>
    class SpiType {
        static_assert((base == address::spi0) || (base == address::spi1), "Invalid base address for SPI register map!");

    public:
        /**
         * @brief Gets the singleton instance.
         *
         * @return SpiType The singleton instance.
         */
        static SpiType& get_instance();

        /**
         * @brief Initializes the SPI controller by taking it out of reset.
         *
         * @pre None
         *
         * @post The SPI controller is ready to be configured.
         */
        void init();

        void deinit();

    private:
        using Registers = SpiRegMap<base>;

        SpiType() : m_flags(0) {};

        enum class Flags : std::uint32_t {
            initialized = 0U,
            number_of_flags
        };

        std::bitset<std::to_underlying(Flags::number_of_flags)> m_flags;
    };

    using Spi0 = SpiType<address::spi0>; /**< @brief SPI0 controller. */
    using Spi1 = SpiType<address::spi1>; /**< @brief SPI1 controller. */

    /** @}*/ // rp2040_spi
}

#include "../../../spi.tpp"

#endif //RP2040_HARDWARE_SPI_H
