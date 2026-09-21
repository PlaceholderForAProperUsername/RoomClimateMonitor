/**
 * @file gpio_def.h
 * @brief The definitions for the GPIO module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 26.07.2026
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

#ifndef RP2040_SYSTEM_GPIO_DEF_H
#define RP2040_SYSTEM_GPIO_DEF_H

#include <cstdint>
#include "rp2040/reg_access.h"

namespace rp2040::system::gpio {
    /**
     * @addtogroup rp2040_gpio
     * @{
     */

    /**
     * @brief The addresses of the gpio module.
     *
     * Each gpio module has three different parts:
     * - IO_Bank: General configuration (func select, etc.)
     * - Pads_Bank: Configuration of electrical parameters (internal resistors, slew rate, etc.)
     * - SIO: Software input output
     */
    namespace address {
        /**
         * @brief The addresses of gpio bank 0.
         */
        namespace gpio0 {
            constexpr std::uintptr_t io_bank0 = 0x40014000U; /**< @brief Base address of the user bank IO registers. */
            constexpr std::uintptr_t pads_bank0 = 0x4001C000U; /**< @brief Base address of the user bank pad control registers. */
            constexpr std::uintptr_t sio = 0xD0000000U; /**< @brief Base address of the software input/output registers. */
        }
    }

    /**
     * @brief The GPIO pins of the RP2040.
     */
    enum class Pins : std::uint8_t {
        gpio0 = 0U,
        gpio1 = 1U,
        gpio2 = 2U,
        gpio3 = 3U,
        gpio4 = 4U,
        gpio5 = 5U,
        gpio6 = 6U,
        gpio7 = 7U,
        gpio8 = 8U,
        gpio9 = 9U,
        gpio10 = 10U,
        gpio11 = 11U,
        gpio12 = 12U,
        gpio13 = 13U,
        gpio14 = 14U,
        gpio15 = 15U,
        gpio16 = 16U,
        gpio17 = 17U,
        gpio18 = 18U,
        gpio19 = 19U,
        gpio20 = 20U,
        gpio21 = 21U,
        gpio22 = 22U,
        gpio23 = 23U,
        gpio24 = 24U,
        gpio25 = 25U,
        gpio26 = 26U,
        gpio27 = 27U,
        gpio28 = 28U,
        gpio29 = 29U,
        number_of_gpio
    };

    /**
     * @brief The pads of the RP2040.
     *
     * The pads include the GPIOs and additionally SWCLK and SWD.
     */
    enum class Pads : std::uint8_t {
        gpio0 = 0U,
        gpio1 = 1U,
        gpio2 = 2U,
        gpio3 = 3U,
        gpio4 = 4U,
        gpio5 = 5U,
        gpio6 = 6U,
        gpio7 = 7U,
        gpio8 = 8U,
        gpio9 = 9U,
        gpio10 = 10U,
        gpio11 = 11U,
        gpio12 = 12U,
        gpio13 = 13U,
        gpio14 = 14U,
        gpio15 = 15U,
        gpio16 = 16U,
        gpio17 = 17U,
        gpio18 = 18U,
        gpio19 = 19U,
        gpio20 = 20U,
        gpio21 = 21U,
        gpio22 = 22U,
        gpio23 = 23U,
        gpio24 = 24U,
        gpio25 = 25U,
        gpio26 = 26U,
        gpio27 = 27U,
        gpio28 = 28U,
        gpio29 = 29U,
        swclk = 30U,
        swd = 31U,
        number_of_pads
    };

    /**
     * @brief The functions a GPIO can be configured for.
     */
    enum class FuncSelect : std::uint32_t {
        spi = 1U,
        uart = 2U,
        i2c = 3U,
        pwm = 4U,
        sio = 5U,
        pio0 = 6U,
        pio1 = 7U,
        clock = 8U, /**< @brief Only available for GPIO20 - GPIO25.  */
        usb = 9U,
        number_of_functions,
    };

    /**
     * @brief Register map for user bank IO registers.
     *
     * The user bank io registers consist of two larger groups of registers: The gpio registers consisting of a status and
     * control register for each gpio and the interrupt registers.
     *
     * @tparam io_bank_base_addr The base address of the user bank IO registers.
     */
    template <std::uintptr_t io_bank_base_addr>
    struct IoBankRegMap {
        static_assert(io_bank_base_addr == address::gpio0::io_bank0, "Invalid base address for IO_BankRegMap.");

        /**
         * @brief General register map to configure and read the status of a GPIO of the user bank IO.
         *
         * @tparam gpio The GPIO to which the regmap belongs.
         */
        template <Pins gpio>
        struct Gpio {
            static_assert(gpio < Pins::number_of_gpio, "Invalid GPIO. There are only 29 user GPIOs.");

            static constexpr std::uint8_t bytes_per_gpio_registers {8U}; /**< @brief The number of bytes required for the ctrl and status registers of a GPIO. */
            static constexpr std::uint8_t offset = static_cast<std::uint8_t>(gpio) * bytes_per_gpio_registers; /**< @brief The offset of the registers of the gpio. */
            static constexpr std::uintptr_t base_addr = io_bank_base_addr + offset; /**< @brief The base address of the registers of the gpio. */
            static constexpr std::uint8_t status_offset = 0x00U; /**< @brief The offset of the status register to the gpio base address. */
            static constexpr std::uint8_t ctrl_offset = 0x04U; /**< @brief The offset of the ctrl register to the gpio base address. */

            /**
             * @brief The status register of the gpio.
             */
            struct Status {
                static constexpr std::uintptr_t addr = base_addr + status_offset; /**< @brief The address of the gpio's status register. */
                using status_reg = reg_access::Reg<addr, reg_access::access_tag::read, std::uint32_t>; /**< @brief The status register. */

                /**
                 * @brief The "interrupt to processors, after override is applied" bit.
                 */
                struct IrqToProc {
                    static constexpr std::uint32_t pos = 26U; /**< @brief The position of the bit in the status register. */
                    /**
                     * @brief The description of the IrqToProc bit.
                     */
                    using BitType = reg_access::BitTypeEnableDisable<status_reg, pos>;
                    /**
                     * @brief The object to handle the IrqToProc bit.
                     */
                    using Bits = status_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the IrqToProc bit.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The "interrupt from pad, before override is applied" bit.
                 */
                struct IrqFromPad {
                    static constexpr std::uint32_t pos = 24U; /**< @brief The position of the bit in the status register. */
                    /**
                     * @brief The description of the IrqFromPad bit.
                     */
                    using BitType = reg_access::BitTypeEnableDisable<status_reg, pos>;
                    /**
                     * @brief The object to handle the IrqFromPad bit.
                     */
                    using Bits = status_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the IrqFromPad bit.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The "input signal to peripheral, after override is applied" bit.
                 */
                struct InToPeri {
                    static constexpr std::uint32_t pos = 19U; /**< @brief The position of the bit in the status register. */
                    /**
                     * @brief The description of the InToPeri bit.
                     */
                    using BitType = reg_access::BitTypeEnableDisable<status_reg, pos>;
                    /**
                     * @brief The object to handle the InToPeri bit.
                     */
                    using Bits = status_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the InToPeri bit.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The "input signal from pad, before override is applied" bit.
                 */
                struct InFromPad {
                    static constexpr std::uint32_t pos = 17U; /**< @brief The position of the bit in the status register. */
                    /**
                     * @brief The description of the InFromPad bit.
                     */
                    using BitType = reg_access::BitTypeEnableDisable<status_reg, pos>;
                    /**
                     * @brief The object to handle the InFromPad bit.
                     */
                    using Bits = status_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the InFromPad bit.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The "output enable to pad, after register override is applied" bit.
                 */
                struct OutEnToPad {
                    static constexpr std::uint32_t pos = 13U; /**< @brief The position of the bit in the status register. */
                    /**
                     * @brief The description of the OEToPad bit.
                     */
                    using BitType = reg_access::BitTypeEnableDisable<status_reg, pos>;
                    /**
                     * @brief The object to handle the OEToPad bit.
                     */
                    using Bits = status_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the OEToPad bit.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The "output enable from selected peripheral, before register override is applied" bit.
                 */
                struct OutEnFromPeri {
                    static constexpr std::uint32_t pos = 12U; /**< @brief The position of the bit in the status register. */
                    /**
                     * @brief The description of the OEFromPeri bit.
                     */
                    using BitType = reg_access::BitTypeEnableDisable<status_reg, pos>;
                    /**
                     * @brief The object to handle the OEFromPeri bit.
                     */
                    using Bits = status_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the OEFromPeri bit.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The "output signal to pad, after register override is applied" bit.
                 */
                struct OutToPad {
                    static constexpr std::uint32_t pos = 9U; /**< @brief The position of the bit in the status register. */
                    /**
                     * @brief The description of the OutToPad bit.
                     */
                    using BitType = reg_access::BitTypeEnableDisable<status_reg, pos>;
                    /**
                     * @brief The object to handle the OutToPad bit.
                     */
                    using Bits = status_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the OutToPad bit.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The "output signal from selected peripheral, before register override is applied" bit.
                 */
                struct OutFromPeri {
                    static constexpr std::uint32_t pos = 8U; /**< @brief The position of the bit in the status register. */
                    /**
                     * @brief The description of the OutFromPeri bit.
                     */
                    using BitType = reg_access::BitTypeEnableDisable<status_reg, pos>;
                    /**
                     * @brief The object to handle the OutFromPeri bit.
                     */
                    using Bits = status_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the OutFromPeri bit.
                     */
                    using Value = BitType::value;
                };
            };

            /**
             * @brief GPIO control register to set the function and overrides.
             */
            struct Ctrl {
                static constexpr std::uintptr_t addr = base_addr + ctrl_offset; /**< @brief The address of the gpio's ctrl register. */
                using ctrl_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The gpio control register. */

                /**
                 * @brief The interrupt override bits.
                 */
                struct IrqOver {
                    /**
                     * @brief The description of the interrupt override bits.
                     */
                    struct BitType {
                        using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                        using T = reg::RegType; /**< @brief The type of the register. */
                        using bit_access = ctrl_reg::reg_access; /**< @brief The access tag defining bit operations. */

                        static constexpr T position = 0x28U; /**< @brief The position of the bits in the register. */
                        static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                        /**
                         * @brief The override values for the interrupt.
                         */
                        enum class value : T {
                            normal = 0x0U, /** @brief Interrupt set to normal. */
                            invert = 0x1U, /** @brief Interrupt is inverted. */
                            low = 0x02U, /** @brief Interrupt is driven low. */
                            high = 0x3U, /** @brief Interrupt is driven high. */
                        };
                    };

                    /**
                     * @brief The object to handle the interrupt override bits.
                     */
                    using Bits = ctrl_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the interrupt override bits.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The input override bits.
                 */
                struct InOver {
                    /**
                     * @brief The description of the input override bits.
                     */
                    struct BitType {
                        using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                        using T = reg::RegType; /**< @brief The type of the register. */
                        using bit_access = ctrl_reg::reg_access; /**< @brief The access tag defining bit operations. */

                        static constexpr T position = 0x16U; /**< @brief The position of the bits in the register. */
                        static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                        /**
                         * @brief The override values for the peripheral input.
                         */
                        enum class value : T {
                            normal = 0x0U, /** @brief Peripheral input set to normal. */
                            invert = 0x1U, /** @brief Peripheral input is inverted. */
                            low = 0x02U, /** @brief Peripheral input is driven low. */
                            high = 0x3U, /** @brief Peripheral input is driven high. */
                        };
                    };

                    /**
                     * @brief The object to handle the input override bits.
                     */
                    using Bits = ctrl_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the input override bits.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The output enable override bits.
                 */
                struct OutEnOver {
                    /**
                     * @brief The description of the output enable override bits.
                     */
                    struct BitType {
                        using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                        using T = reg::RegType; /**< @brief The type of the register. */
                        using bit_access = ctrl_reg::reg_access; /**< @brief The access tag defining bit operations. */

                        static constexpr T position = 0x12U; /**< @brief The position of the bits in the register. */
                        static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                        /**
                         * @brief The override values for the output enable bits.
                         */
                        enum class value : T {
                            normal = 0x0U, /** @brief Output enable from peripheral signal selected by funcsel is normal. */
                            invert = 0x1U, /** @brief Output enable from peripheral signal selected by funcsel is inverted. */
                            low = 0x02U, /** @brief Disable output. */
                            high = 0x3U,  /** @brief Enable output. */
                        };
                    };

                    /**
                     * @brief The object to handle the output enable override bits.
                     */
                    using Bits = ctrl_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the output enable override bits.
                     */
                    using Value = BitType::value;
                };


                /**
                 * @brief The output override bits.
                 */
                struct OutOver {
                    /**
                     * @brief The description of the output override bits.
                     */
                    struct BitType {
                        using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                        using T = reg::RegType; /**< @brief The type of the register. */
                        using bit_access = ctrl_reg::reg_access; /**< @brief The access tag defining bit operations. */

                        static constexpr T position = 0x8U; /**< @brief The position of the bits in the register. */
                        static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                        /**
                         * @brief The override values for the output bits.
                         */
                        enum class value : T {
                            normal = 0x0U, /** @brief Output from peripheral signal selected by funcsel is normal. */
                            invert = 0x1U, /** @brief Output from peripheral signal selected by funcsel is inverted. */
                            low = 0x02U, /** @brief Output is driven low. */
                            high = 0x3U,  /** @brief Output is driven high. */
                        };
                    };

                    /**
                     * @brief The object to handle the output override bits.
                     */
                    using Bits = ctrl_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the output override bits.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The function select bits.
                 */
                struct FuncSel {
                    /**
                     * @brief The description of the function select bits.
                     */
                    struct BitType {
                        using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                        using T = reg::RegType; /**< @brief The type of the register. */
                        using bit_access = ctrl_reg::reg_access;

                        static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                        static constexpr T mask = (0x1FU << position); /**< @brief The mask of the bits. */

                        /**
                         * @brief The functions the GPIO can be configured for.
                         *
                         * GPIO20 - GPIO25 can additionally be configured for clock input/output.
                         */
                        using value = FuncSelect;
                    };
                    /**
                     * @brief The object to handle the function select bits.
                     */
                    using Bits = ctrl_reg::template Bits<BitType>;
                    /**
                     * @brief The values of the function select bits.
                     */
                    using Value = BitType::value;
                };
            };
        };

        /**
         * @brief The base of the interrupt register.
         */
        enum class InterruptBase : std::uint32_t {
            raw_interrupt = 0x0F0U,
            proc0 = 0x100U,
            proc1 = 0x130U,
            dormant_wake = 0x160U,
        };

        /**
         * @brief The different interrupt register types.
         */
        enum class InterruptType : std::uint32_t {
            raw = 0x00U,
            enable = 0x00U,
            force = 0x10U,
            status = 0x20U,
        };

        /**
         * @brief The bits to check the interrupts for an GPIO.
         *
         * The interrupt registers are divided in four major groups: raw interrupts, proc0, proc1 and dormant_wake. Proc0,
         * proc1 and dormant_wake have three subgroups: interrupt enable, interrupt force and interrupt status.
         *
         * All these groups and subgroups have the same basic layout, they only differ in bit access type. The first register
         * in each group starts with the interrupts for GPIO0 and continues from there. There are four interrupt triggers
         * for each GPIO: level low, level high, edge low and edge high.
         *
         * @tparam base The base of the interrupt register.
         * @tparam type The type of the interrupt register.
         */
        template <InterruptBase base, InterruptType type>
        struct Interrupt {
            static_assert((base == InterruptBase::raw_interrupt && type == InterruptType::raw) ||
                    ((base == InterruptBase::proc0 || base == InterruptBase::proc1 || base == InterruptBase::dormant_wake) &&
                    (type == InterruptType::enable || type == InterruptType::force || type == InterruptType::status)), "Invalid combination of interrupt base and type");

            static constexpr std::uint32_t base_offset =  static_cast<std::uint32_t>(base) + static_cast<std::uint32_t>(type); /**< @brief The offset of the target register block to the USER_BANK_BASE address. */

            /**
             * @brief The interrupt bits of the gpio.
             *
             * @tparam gpio The gpio for which the interrupt is to be configured/status of interrupt to be read.
             */
            template <Pins gpio>
            struct Gpio {
                static_assert(gpio < Pins::number_of_gpio, "Invalid GPIO.");

                static constexpr std::uint32_t gpio_per_register = 8U; /**< @brief The maximum number of gpio per interrupt register. */
                static constexpr std::uint32_t bytes_per_register = 4U; /**< @brief A register is 4 byte wide. */
                static constexpr std::uint32_t gpio_reg_offset = (static_cast<std::uint32_t>(gpio) / gpio_per_register) * bytes_per_register; /**< @brief The offset of the register to which the gpio belongs from the first interrupt register of the register block. */
                static constexpr std::uint32_t bits_per_gpio = 4U; /**< @brief There are four bits per gpio. */
                static constexpr std::uint32_t gpio_bit_offset = (static_cast<std::uint32_t>(gpio) % gpio_per_register) * bits_per_gpio; /**< @brief The bit offset of the interrupt bits of the gpio. */

                static constexpr std::uintptr_t addr = io_bank_base_addr + base_offset + gpio_reg_offset; /**< @brief The address of the specific interrupt register of the gpio. */

                /**
                 * @brief The interrupt register of the GPIO.
                 *
                 * The read-write access depends on the interrupt type. Raw interrupt registers have mixed R0-WC bits, Status registers
                 * are read-only and enable and force interrupt registers have read-write access.
                 */
                using interrupt_reg = decltype([]() {
                    if constexpr (type == InterruptType::raw) {
                        return std::type_identity_t<reg_access::Reg<addr, reg_access::access_tag::mixed_access, std::uint32_t>>();
                    } else if constexpr (type == InterruptType::status) {
                        return std::type_identity_t<reg_access::Reg<addr, reg_access::access_tag::read, std::uint32_t>>();
                    } else {
                        return std::type_identity_t<reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>>();
                    }
                }());

                /**
                 * @brief The edge high bit of the gpio's interrupt register.
                 */
                struct EdgeHigh {
                    static constexpr std::uint32_t edge_high_bit_offset = 3U; /**< @brief The offset from the gpio base position. */

                    /**
                     * @brief Description of the edge high bits.
                     *
                     * The read-write access depends on the interrupt type. Raw interrupt edge high bits are write-clear, status
                     * interrupts edge high bits are read-only and enable and force interrupt edge high bits have read-write access.
                     */
                    using BitType = decltype([]() {
                        if constexpr (type == InterruptType::raw) {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + edge_high_bit_offset, reg_access::access_tag::write_clear>>();
                        } else if constexpr (type == InterruptType::status) {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + edge_high_bit_offset, reg_access::access_tag::read>>();
                        } else {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + edge_high_bit_offset, reg_access::access_tag::read_write>>();
                        }
                    }());
                    /**
                     * @brief The object to handle the edge high interrupt bits.
                     */
                    using Bits = interrupt_reg::template Bits<BitType>;
                    /**
                     * @brief The value of the edge high interrupt bits.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The edge low bit of the gpio's interrupt register.
                 */
                struct EdgeLow {
                    static constexpr std::uint32_t edge_low_bit_offset = 2U; /**< @brief The offset from the gpio base position. */

                    /**
                     * @brief Description of the edge low bits.
                     *
                     * The read-write access depends on the interrupt type. Raw interrupt edge low bits are write-clear, status
                     * interrupts edge low bits are read-only and enable and force interrupt edge low bits have read-write access.
                     */
                    using BitType = decltype([]() {
                        if constexpr (type == InterruptType::raw) {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + edge_low_bit_offset, reg_access::access_tag::write_clear>>();
                        } else if constexpr (type == InterruptType::status) {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + edge_low_bit_offset, reg_access::access_tag::read>>();
                        } else {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + edge_low_bit_offset, reg_access::access_tag::read_write>>();
                        }
                    }());
                    /**
                     * @brief The object to handle the edge low interrupt bits.
                     */
                    using Bits = interrupt_reg::template Bits<BitType>;
                    /**
                     * @brief The value of the edge low interrupt bits.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The level high bit of the gpio's interrupt register.
                 */
                struct LevelHigh {
                    static constexpr std::uint32_t level_high_bit_offset = 1U; /**< @brief The offset from the gpio base position. */

                    /**
                     * @brief Description of the level high bits.
                     *
                     * The read-write access depends on the interrupt type. Raw interrupt level high bits and status
                     * interrupts level high bits are read-only, and enable and force interrupt level high bits have read-write access.
                     */
                    using BitType = decltype([]() {
                        if constexpr (type == InterruptType::raw) {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + level_high_bit_offset, reg_access::access_tag::read>>();
                        } else if constexpr (type == InterruptType::status) {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + level_high_bit_offset, reg_access::access_tag::read>>();
                        } else {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + level_high_bit_offset, reg_access::access_tag::read_write>>();
                        }
                    }());
                    /**
                     * @brief The object to handle the level high interrupt bits.
                     */
                    using Bits = interrupt_reg::template Bits<BitType>;
                    /**
                     * @brief The value of the level high interrupt bits.
                     */
                    using Value = BitType::value;
                };

                /**
                 * @brief The level low bit of the gpio's interrupt register.
                 */
                struct LevelLow {
                    static constexpr std::uint32_t level_low_bit_offset = 0U; /**< @brief The offset from the gpio base position. */

                    /**
                     * @brief Description of the level low bits.
                     *
                     * The read-write access depends on the interrupt type. Raw interrupt level low bits and status
                     * interrupts level low bits are read-only, and enable and force interrupt level low bits have read-write access.
                     */
                    using BitType = decltype([]() {
                        if constexpr (type == InterruptType::raw) {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + level_low_bit_offset, reg_access::access_tag::read>>();
                        } else if constexpr (type == InterruptType::status) {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + level_low_bit_offset, reg_access::access_tag::read>>();
                        } else {
                            return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpio_bit_offset + level_low_bit_offset, reg_access::access_tag::read_write>>();
                        }
                    }());
                    /**
                     * @brief The object to handle the level low interrupt bits.
                     */
                    using Bits = interrupt_reg::template Bits<BitType>;
                    /**
                     * @brief The value of the level low interrupt bits.
                     */
                    using Value = BitType::value;
                };
            };
        };

        /**
         * @brief The raw interrupt registers.
         */
        using RawInterrupt = Interrupt<InterruptBase::raw_interrupt, InterruptType::raw>;

        /**
         * @brief The proc0 interrupt block.
         */
        struct Proc0 {
            /**
             * @brief The base for the proc0 interrupts.
             *
             * @tparam type The type of the interrupts registers.
             */
            template <InterruptType type>
            using Base = Interrupt<InterruptBase::proc0, type>;

            using Enable = Base<InterruptBase::enable>; /**< @brief The enable interrupt registers of proc0. */
            using Force = Base<InterruptBase::force>; /**< @brief The force interrupt registers of proc0. */
            using Status = Base<InterruptBase::status>; /**< @brief The interrupt status registers of proc0. */
        };

        /**
         * @brief The proc1 interrupt block.
         */
        struct Proc1 {
            /**
             * @brief The base for the proc1 interrupts.
             *
             * @tparam type The type of the interrupts registers.
             */
            template <InterruptType type>
            using Base = Interrupt<InterruptBase::proc1, type>;

            using Enable = Base<InterruptBase::enable>; /**< @brief The enable interrupt registers of proc1. */
            using Force = Base<InterruptBase::force>; /**< @brief The force interrupt registers of proc1. */
            using Status = Base<InterruptBase::status>; /**< @brief The interrupt status registers of proc1. */
        };

        /**
         * @brief The dormant_wake interrupt block.
         */
        struct DormantWake {
            /**
             * @brief The base for the dormant_wake interrupts.
             *
             * @tparam type The type of the interrupts registers.
             */
            template <InterruptType type>
            using Base = Interrupt<InterruptBase::dormant_wake, type>;

            using Enable = Base<InterruptBase::enable>; /**< @brief The enable interrupt registers of dormant_wake. */
            using Force = Base<InterruptBase::force>; /**< @brief The force interrupt registers of dormant_wake. */
            using Status = Base<InterruptBase::status>; /**< @brief The interrupt status registers of dormant_wake. */
        };
    };

    /**
     * @brief The user bank pad control registers.
     *
     * @tparam PadsBaseAddr The base address of the user bank pad control registers.
     */
    template <std::uintptr_t PadsBaseAddr>
    struct PadsBankRegMap {
        static_assert(PadsBaseAddr == address::gpio0::pads_bank0, "Invalid address for PadsBaseAddr.");

        static constexpr std::uint8_t voltageSelectOffset = 0x00U; /**< @brief The offset of the voltage select register. */
        static constexpr std::uint8_t padRegisterStartOffset = 0x04U; /**< @brief The offset of the first pad control register. */

        /**
         * @brief The voltage select register.
         */
        struct voltage_select {
            static constexpr std::uintptr_t addr = PadsBaseAddr + voltageSelectOffset; /**< @brief Base address of the voltage select register. */
            using voltage_select_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The voltage select register. */

            /**
             * @brief The voltage select bits as a bit field.
             */
            struct VoltageSelectBitField {
                using reg = voltage_select_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg::reg_access;

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the bits. */

                /**
                 * @brief The voltage select control per bank control.
                 */
                enum class value : T {
                    VOLTAGE_3_3 = 0x0U, /**< @brief Set the voltage to 3.3V. */
                    VOLTAGE_1_8 = 0x1U, /**< @brief Set the voltage to 1.8V. */
                };
            };

            /**
             * @brief The voltage select bits.
             */
            using voltageSelect_bits = voltage_select_reg::template Bits<VoltageSelectBitField>;
        };

        /**
         * @brief The register to configure a pad.
         *
         * @tparam pad The pad to be configured.
         */
        template <Pads pad>
        struct padX {
            static_assert(pad < Pads::number_of_pads, "Invalid Pad. For valid options see enum Pads.");

            static constexpr std::uint8_t bytesPerRegister = 0x04U; /**< @brief The RP2040 is a 32 bit architecture and every register is therefore 4 bytes. */
            static constexpr std::uint8_t padOffset = padRegisterStartOffset + static_cast<std::uint8_t>(pad) * bytesPerRegister; /**< @brief The offset of the specified pad from the first pad (GPIO0). */

            static constexpr std::uintptr_t addr = PadsBaseAddr + padOffset; /**< @brief The address of the pad register to be configured. */
            using ctrl_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The pad register to be configured. */

            /**
             * @brief The output disable bit as a bit field.
             *
             * Disabling the output via this bit has priority over output enable from peripherals.
             */
            struct OutputDisableBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg::reg_access;

                static constexpr T position = 0x07U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the bits. */

                /**
                 * @brief The output disable options.
                 */
                enum class value : T {
                    Reset = 0x0U, /**< @brief Resets the output disable bit and allows peripherals to enable output. */
                    DisableOutput = 0x1U, /**< @brief Disables the output. */
                };
            };

            /**
             * @brief The output disable bit.
             */
            using outputDisable_bits = ctrl_reg::template Bits<OutputDisableBitField>;

            /**
             * @brief The input enable bit field.
             */
            using InputEnableBitField = rp2040::reg_access::BitTypeEnableDisable<ctrl_reg, 0x6U>;
            /**
             * @brief The input enable bit.
             */
            using inputEnable_bits = ctrl_reg::template Bits<InputEnableBitField>;

            /**
             * @brief The bit field to set the drive strength of the pad.
             */
            struct DriveBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg::reg_access;

                static constexpr T position = 0x04U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                /**
                 * @brief The drive strength options.
                 */
                enum class value : T {
                    Drive_2MA = 0x0U,
                    Drive_4MA = 0x01U,
                    Drive_8MA = 0x02U,
                    Drive_12MA = 0x03U,
                };
            };

            /**
             * @brief The drive strength bits.
             */
            using drive_bits = ctrl_reg::template Bits<DriveBitField>;

            /**
             * @brief The pull up enable bit as a bit field.
             */
            using PullUpEnableBitField = rp2040::reg_access::BitTypeEnableDisable<ctrl_reg, 0x3U>;
            /**
             * @brief The pull up enable bit.
             */
            using pullUpEnable_bits = ctrl_reg::template Bits<PullUpEnableBitField>;

            /**
             * @brief The pull down enable bit as a bit field.
             */
            using PullDownEnableBitField = rp2040::reg_access::BitTypeEnableDisable<ctrl_reg, 0x2U>;
            /**
             * @brief The pull down enable bit.
             */
            using pullDownEnable_bits = ctrl_reg::template Bits<PullDownEnableBitField>;

            /**
             * @brief The schmitt bit as a bit field to enable/disable the schmitt trigger.
             */
            using SchmittBitField = rp2040::reg_access::BitTypeEnableDisable<ctrl_reg, 0x1U>;
            /**
             * @brief The schmitt bit to enable/disable the schmitt trigger.
             */
            using schmitt_bits = ctrl_reg::template Bits<SchmittBitField>;

            /**
             * @brief The slew rate control bit as a bit field.
             */
            struct SlewFastBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg::reg_access;

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the bits. */

                /**
                 * @brief The slew rate control options.
                 */
                enum class value : T {
                    Slow = 0x0U,
                    Fast = 0x1U,
                };
            };

            /**
             * @brief The slew rate control bit.
             */
            using slewFast_bits = ctrl_reg::template Bits<SlewFastBitField>;
        };
    };

    /**
     * @brief Register map to handle software input/output.
     *
     * @tparam SIOBaseAddr The base address of the software input/output registers.
     */
    template <std::uintptr_t SIOBaseAddr>
    struct SIORegMap {
        static_assert(SIOBaseAddr == address::gpio0::sio, "Invalid address for SIOBaseAddr.");

        static constexpr std::uint32_t cpuidOffset = 0x000U; /**< @brief Offset of the cpuid register to the sio base address. */
        static constexpr std::uint32_t gpioInOffset = 0x004U; /**< @brief Offset of the gpio_in register to the sio base address. */
        static constexpr std::uint32_t gpioOutOffset = 0x010U; /**< @brief Offset of the gpio_out register to the sio base address. */
        static constexpr std::uint32_t gpioOutEnOffset = 0x020U; /**< @brief Offset of the gpio_oe register to the sio base address. */

        /**
         * @brief The processor core identifier register.
         */
        struct cpuid {
            static constexpr std::uintptr_t addr = SIOBaseAddr + cpuidOffset; /**< @brief The base address of the cpuid register. */
            using cpuid_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read, std::uint32_t>; /**< @brief The the cpuid register. */

            /**
             * @brief The processor core identifier bits as a bit field.
             */
            struct CPUIDBitField {
                using reg = cpuid_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg::reg_access;

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0xFFFFFFFFU << position); /**< @brief The mask of the bits. */

                /**
                 * @brief The value depends on from which processor core this register is read.
                 */
                enum class value : T {
                    PROC0 = 0x0U, /**< @brief Value when read from processor core 0. */
                    PROC1 = 0x1U, /**< @brief Value when read from processor core 1. */
                };
            };

            /**
             * @brief The processor core identifier bits.
             */
            using cpuid_bits = cpuid_reg::template Bits<CPUIDBitField>;
        };

        /**
         * @brief The gpio input register.
         */
        struct input {
            static constexpr std::uintptr_t addr = SIOBaseAddr + gpioInOffset; /**< @brief The base address of the sio input register. */
            using input_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read, std::uint32_t>; /**< @brief The sio input register. */

            /**
             * @brief The gpio input bit as a bit field.
             *
             * @tparam gpio The GPIO where the input value is to be get.
             */
            template <Pins gpio>
            requires (gpio < Pins::number_of_gpio)
            using InputBitField = rp2040::reg_access::BitTypeHighLow<input_reg, static_cast<input_reg::RegType>(gpio)>;
            /**
             * @brief The input bit of the gpio.
             *
             * @tparam gpio The GPIO where the input value is to be get.
             */
            template <Pins gpio>
            using input_bits = input_reg::template Bits<InputBitField<gpio>>;
        };

        /**
         * @brief The gpio output/output enable registers.
         *
         * The output registers comprises four different registers: a regular register and 3 atomic access registers (set, clear and xor).
         * These registers are all used to configure either the output value or the output enable status, depending on the base address
         * non-type template parameter. The output and output enable register are in an identical format, therefore these registers
         * share a template.
         *
         * @tparam baseOffset Base offset of either the output or output enable registers to SIO_Base address.
         */
        template <std::uint32_t baseOffset>
        struct OutputRegMap {
            static_assert((baseOffset == gpioOutOffset) || (baseOffset == gpioOutEnOffset), "Invalid base offset for output.");

            static constexpr std::uint32_t setOffset = 0x04U; /**< @brief Offset of the atomic set register. */
            static constexpr std::uint32_t clearOffset = 0x08U; /**< @brief Offset of the atomic clear register. */
            static constexpr std::uint32_t xorOffset = 0x0C; /**< @brief Offset of the atomic xor register. */

            static constexpr std::uintptr_t outputBaseAddr = SIOBaseAddr + baseOffset; /**< @brief Base address of either the output or output enable register. */

            /**
             * @brief The output/output enable value register.
             */
            struct value {
                using value_reg = rp2040::reg_access::Reg<outputBaseAddr, rp2040::reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The output or output enable register. */

                /**
                 * @brief The output/output enable value bits as a Bitfield.
                 *
                 * The options only differ in the enum values. High/Low is more reasonable for the output value, while
                 * Enable/Disable is more reasonable to set output enable bits.
                 *
                 * @tparam gpio The gpio to be configured.
                 */
                template <Pins gpio>
                using ValueBitField = decltype([] () {
                    static_assert(gpio < Pins::number_of_gpio);
                    if constexpr(baseOffset == gpioOutOffset) {
                        return std::type_identity_t<rp2040::reg_access::BitTypeHighLow<value_reg, static_cast<value_reg::RegType>(gpio)>>();
                    } else if constexpr (baseOffset == gpioOutEnOffset) {
                        return std::type_identity_t<rp2040::reg_access::BitTypeEnableDisable<value_reg, static_cast<value_reg::RegType>(gpio)>>();
                    }
                }());

                /**
                 * @brief The output/output enable value bits.
                 *
                 * @tparam gpio The gpio to be configured.
                 */
                template <Pins gpio>
                using value_bits = value_reg::template Bits<ValueBitField<gpio>>;
            };

            /**
             * @brief The output set register.
             *
             * This is an atomic register.
             */
            struct set {
                static constexpr std::uintptr_t outputSetAddr = outputBaseAddr + setOffset; /**< @brief Base address of the atomic set register. */
                using set_reg = rp2040::reg_access::Reg<outputSetAddr, rp2040::reg_access::access_tag::atomic_set_mask, std::uint32_t>; /**< @brief The atomic set register. */

                /**
                 * @brief The output set bits.
                 *
                 * This is an atomic operation.
                 *
                 * @tparam gpio The gpio for which the output is to be set.
                 */
                template <Pins gpio>
                using set_bits = set_reg::template Bits<rp2040::reg_access::BitTypeAtomicBitOp<set_reg, static_cast<set_reg::RegType>(gpio)>>;
            };

            /**
             * @brief The output clear register.
             *
             * This is an atomic register.
             */
            struct clear {
                static constexpr std::uintptr_t outputClearAddr = outputBaseAddr + clearOffset; /**< @brief Base address of the atomic clear register. */
                using clear_reg = rp2040::reg_access::Reg<outputClearAddr, rp2040::reg_access::access_tag::atomic_clear, std::uint32_t>; /**< @brief The atomic clear register. */

                /**
                 * @brief The output clear bits.
                 *
                 * This is an atomic operation.
                 *
                 * @tparam gpio The gpio for which the output is to be cleared.
                 */
                template <Pins gpio>
                using clear_bits = clear_reg::template Bits<rp2040::reg_access::BitTypeAtomicBitOp<clear_reg, static_cast<clear_reg::RegType>(gpio)>>;
            };

            /**
            * @brief The output toggle register.
            *
            * This is an atomic register.
            */
            struct toggle {
                static constexpr std::uintptr_t outputXorAddr = outputBaseAddr + xorOffset; /**< @brief Base address of the atomic xor register. */
                using toggle_reg = rp2040::reg_access::Reg<outputXorAddr, rp2040::reg_access::access_tag::atomic_xor, std::uint32_t>; /**< @brief The atomic xor register. */

                /**
                 * @brief The output toggle bits.
                 *
                 * This is an atomic operation.
                 *
                 * @tparam gpio The gpio for which the output is to be toggled.
                 */
                template <Pins gpio>
                using toggle_bits = toggle_reg::template Bits<rp2040::reg_access::BitTypeAtomicBitOp<toggle_reg, static_cast<toggle_reg::RegType>(gpio)>>;
            };
        };

        using output = OutputRegMap<gpioOutOffset>; /**< @brief The output registers. */
        using outputEnable = OutputRegMap<gpioOutEnOffset>; /**< @brief The output enable registers. */
    };

    /** @} */ // rp2040_gpio
} // rp2040::system::gpio

#endif //RP2040_SYSTEM_GPIO_DEF_H
