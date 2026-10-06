/**
 * @file spi_def.h
 * @brief The definitions of the SPI module of the RP2040.
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


#ifndef RP2040_HARDWARE_SPI_DEF_H
#define RP2040_HARDWARE_SPI_DEF_H

#include <cstdint>

namespace rp2040::hardware::spi {
    /**
     * @addtogroup rp2040_spi
     * @{
     */

    /**
     * @brief The base address of the SPI controller.
     */
    namespace address {
        constexpr std::uintptr_t spi0 = 0x4003C000; /**< @brief The base address of the SPI0 controller. */
        constexpr std::uintptr_t spi1 = 0x40040000; /**< @brief The base address of the SPI1 controller. */
    }


    /** @}*/ // rp2040_spi
}

#endif // RP2040_HARDWARE_SPI_DEF_H
