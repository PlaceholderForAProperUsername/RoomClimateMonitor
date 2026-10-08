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

#include "rp2040/reg_access.h"

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


    /**
     * @brief The registers of the SPI controller.
     *
     * @tparam base Base address of the SPI controller
     */
    template <std::uintptr_t base>
    struct SpiRegMap {
        static_assert((base == address::spi0) || (base == address::spi1), "Invalid base address for SPI register map!");

        /**
         * @brief The register offsets from the base address.
         */
        struct Offset {
            static constexpr std::uint32_t cr0 = 0x00U; /**< @brief Offset of the control register 0. */
            static constexpr std::uint32_t cr1 = 0x04U; /**< @brief Offset of the control register 1. */
            static constexpr std::uint32_t dr = 0x08U; /**< @brief Offset of the data register. */
            static constexpr std::uint32_t sr = 0x0CU; /**< @brief Offset of the status register . */
            static constexpr std::uint32_t cpsr = 0x10U; /**< @brief Offset of the clock prescale register. */
        };

        /**
         * @brief Control register 0.
         */
        struct Cr0 {
            static constexpr std::uintptr_t addr = base + Offset::cr0; /**< @brief Address of the control register 0. */
            using cr0_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The control register 0. */

            /**
             * @brief Serial clock rate bits.
             *
             * @tparam clock_rate The serial clock rate value.
             */
            template <std::uint8_t clock_rate>
            struct Scr {
                /**
                 * @brief Description of the serial clock rate bits.
                 */
                struct BitType {
                    using reg = cr0_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = cr0_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 8U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0xFFU << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Value of the serial clock rate bits.
                     */
                    enum class value : T {
                        val = clock_rate,
                    };
                };
                using Bits = cr0_reg::template Bits<BitType>; /**< @brief Object to handle the serial clock rate bits. */
                using Value = BitType::value; /**< @brief Values of the serial clock rate bits. */
            };

            /**
             * @brief Clock phase, only applicable to motorola spi frame format.
             */
            struct Sph {
                static constexpr std::uint32_t pos = 7U; /**< @brief Position of the clock phase bit in the cr0 register. */
                using BitType = reg_access::BitTypeHighLow<cr0_reg, pos>; /**< @brief Description of the clock phase bit. */
                using Bits = cr0_reg::template Bits<BitType>; /**< @brief Object to handle the clock phase bit. */
                using Value = BitType::value; /**< @brief Values of the clock phase bit. */
            };

            /**
             * @brief Clock polarity, only applicable to motorola spi frame format.
             */
            struct Spo {
                static constexpr std::uint32_t pos = 6U; /**< @brief Position of the clock polarity bit in the cr0 register. */
                using BitType = reg_access::BitTypeHighLow<cr0_reg, pos>; /**< @brief Description of the clock polarity bit. */
                using Bits = cr0_reg::template Bits<BitType>; /**< @brief Object to handle the clock polarity bit. */
                using Value = BitType::value; /**< @brief Values of the clock polarity bit. */
            };

            /**
             * @brief Frame format bits.
             */
            struct Frf {
                /**
                 * @brief Description of the frame format bits.
                 */
                struct BitType {
                    using reg = cr0_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = cr0_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 4U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x3U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Values of the frame format bits.
                     */
                    enum class value : T {
                        motorola = 0x0U,
                        ti = 0x1U,
                        national_microwire = 0x2U,
                    };
                };
                using Bits = cr0_reg::template Bits<BitType>; /**< @brief Object to handle the frame format bits. */
                using Value = BitType::value; /**< @brief Values of the frame format bits. */
            };

            /**
             * @brief Data size select bits.
             */
            struct Dss {
                /**
                 * @brief Description of the data size select bits.
                 */
                struct BitType {
                    using reg = cr0_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = cr0_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0xFU << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Values of the data size select bits.
                     */
                    enum class value : T {
                        data_4bits = 0x3U,
                        data_5bits = 0x4U,
                        data_6bits = 0x5U,
                        data_7bits = 0x6U,
                        data_8bits = 0x7U,
                        data_9bits = 0x8U,
                        data_10bits = 0x9U,
                        data_11bits = 0xAU,
                        data_12bits = 0xBU,
                        data_13bits = 0xCU,
                        data_14bits = 0xDU,
                        data_15bits = 0xEU,
                        data_16bits = 0xFU,
                    };
                };
                using Bits = cr0_reg::template Bits<BitType>; /**< @brief Object to handle the data size select bits. */
                using Value = BitType::value; /**< @brief Values of the data size select bits. */
            };
        };

        /**
         * @brief Control register 1.
         */
        struct Cr1 {
            static constexpr std::uintptr_t addr = base + Offset::cr1; /**< @brief Address of the control register 1. */
            using cr1_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The control register 1. */

            /**
             * @brief Slave-mode output disable bit.
             */
            struct Sod {
                /**
                 * @brief Description of the slave-mode output disable bit.
                 */
                struct BitType {
                    using reg = cr1_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = cr1_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 3U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Values of the slave-mode output disable bit.
                     */
                    enum class value : T {
                        output_enable = 0x0U,
                        output_disable = 0x1U,
                    };
                };
                using Bits = cr1_reg::template Bits<BitType>; /**< @brief Object to handle the slave-mode output disable bit. */
                using Value = BitType::value; /**< @brief Values of the slave-mode output disable bit. */
            };

            /**
             * @brief Master or slave mode select bit.
             *
             * This bit can only be modified, when the controller is disabled. @see Sse
             */
            struct Ms {
                /**
                 * @brief Description of the master or slave mode select bit.
                 */
                struct BitType {
                    using reg = cr1_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = cr1_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 2U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Values of the master or slave mode select bit.
                     */
                    enum class value : T {
                        master = 0x0U, /**< @brief I2C controller is configured to be a master. */
                        slave = 0x1U, /**< @brief I2C controller is configured to be a slave. */
                    };
                };
                using Bits = cr1_reg::template Bits<BitType>; /**< @brief Object to handle the master or slave mode select bit. */
                using Value = BitType::value; /**< @brief Values of the master or slave mode select bit. */
            };

            /**
             * @brief Synchronous serial port enable bit.
             */
            struct Sse {
                static constexpr std::uint32_t pos = 1U; /**< @brief Position of the sse bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<cr1_reg, pos>; /**< @brief Description of the sse bit. */
                using Bits = cr1_reg::template Bits<BitType>; /**< @brief Object to handle the sse bit. */
                using Value = BitType::value; /**< @brief Values of the sse bit. */
            };

            /**
             * @brief Loop back mode bit.
             */
            struct Lbm {
                static constexpr std::uint32_t pos = 0U; /**< @brief Position of the lbm bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<cr1_reg, pos>; /**< @brief Description of the lbm bit. */
                using Bits = cr1_reg::template Bits<BitType>; /**< @brief Object to handle the lbm bit. */
                using Value = BitType::value; /**< @brief Values of the lbm bit. */
            };
        };
    };

    /** @}*/ // rp2040_spi
}

#endif // RP2040_HARDWARE_SPI_DEF_H
