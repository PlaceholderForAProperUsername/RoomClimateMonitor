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
     * @brief Register map for user bank IO registers.
     *
     * @tparam ioBankBaseAddr The base address of the user bank IO registers.
     */
    template <std::uintptr_t ioBankBaseAddr>
    struct IO_BankRegMap {
        static_assert(ioBankBaseAddr == address::gpio0::io_bank0, "Invalid base address for IO_BankRegMap.");

        /**
         * @brief General register map to configure and read the status of a GPIO of the user bank IO.
         *
         * @tparam gpio The GPIO to which the regmap belongs.
         */
        template <Pins gpio>
        struct GPIOX_RegMap {
            static_assert(gpio < Pins::number_of_gpio, "Invalid GPIO. There are only 29 user GPIOs.");

            static constexpr std::uint8_t BytesPerGPIORegisters {8U}; /**< @brief The number of bytes required for the ctrl and status registers of a GPIO. */
            static constexpr std::uint8_t offset = static_cast<std::uint8_t>(gpio) * BytesPerGPIORegisters; /**< @brief The offset of the registers of the gpio. */
            static constexpr std::uintptr_t base_addr = ioBankBaseAddr + offset; /**< @brief The base address of the registers of the gpio. */
            static constexpr std::uint8_t statusOffset = 0x00U; /**< @brief The offset of the status register to the gpio base address. */
            static constexpr std::uint8_t ctrlOffset = 0x04U; /**< @brief The offset of the ctrl register to the gpio base address. */

            /**
             * @brief The status register of the gpio.
             */
            struct status {
                static constexpr std::uintptr_t addr = base_addr + statusOffset; /**< @brief The address of the gpio's status register. */
                using status_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read, std::uint32_t>; /**< @brief The status register. */

                /**
                 * @brief Bitfield for the status bit "interrupt to processors, after override is applied".
                 */
                using IRQToProcBitfield = rp2040::reg_access::BitTypeEnableDisable<status_reg, 26U, reg_access::access_tag::read>;
                /**
                 * @brief The status bit "interrupt to processors, after override is applied".
                 */
                using irqToProc_bits = status_reg::template Bits<IRQToProcBitfield>;

                /**
                 * @brief Bitfield for the status bit "interrupt from pad before override is applied".
                 */
                using IRQFromPadBitField = rp2040::reg_access::BitTypeEnableDisable<status_reg, 24U, reg_access::access_tag::read>;
                /**
                 * @brief The status bit "interrupt from pad before override is applied".
                 */
                using irqFromPad_bits = status_reg::template Bits<IRQFromPadBitField>;

                /**
                 * @brief Bitfield for the status bit "input signal to peripheral, after override is applied".
                 */
                using IntToPeriBitField = rp2040::reg_access::BitTypeEnableDisable<status_reg, 19U, reg_access::access_tag::read>;
                /**
                 * @brief The status bit "input signal to peripheral, after override is applied".
                 */
                using intToPeri_bits = status_reg::template Bits<IntToPeriBitField>;

                /**
                 * @brief Bitfield for the status bit "input signal from pad, before override is applied".
                 */
                using IntFromPadBitField = rp2040::reg_access::BitTypeEnableDisable<status_reg, 17U, reg_access::access_tag::read>;
                /**
                 * @brief The status bit "input signal from pad, before override is applied".
                 */
                using intFromPad_bits = status_reg::template Bits<IntFromPadBitField>;

                /**
                 * @brief Bitfield for the status bit "output enable to pad after register override is applied".
                 */
                using OEToPadBitField = rp2040::reg_access::BitTypeEnableDisable<status_reg, 13U, reg_access::access_tag::read>;
                /**
                 * @brief The status bit "output enable to pad after register override is applied".
                 */
                using oeToPad_bits = status_reg::template Bits<OEToPadBitField>;

                /**
                 * @brief Bitfield for the status bit "output enable from selected peripheral, before register override is applied".
                 */
                using OEFromPeriBitField = rp2040::reg_access::BitTypeEnableDisable<status_reg, 12U, reg_access::access_tag::read>;
                /**
                 * @brief The status bit "output enable from selected peripheral, before register override is applied".
                 */
                using oeFromPeri_bits = status_reg::template Bits<OEFromPeriBitField>;

                /**
                 * @brief Bitfield for the status bit "output signal to pad after register override is applied".
                 */
                using OutToPadBitField = rp2040::reg_access::BitTypeEnableDisable<status_reg, 9U, reg_access::access_tag::read>;
                /**
                 * @brief The status bit "output signal to pad after register override is applied".
                 */
                using outToPad_bits = status_reg::template Bits<OutToPadBitField>;

                /**
                 * @brief Bitfield for the status bit "output signal from selected peripheral, before register override is applied".
                 */
                using OutFromPeriField = rp2040::reg_access::BitTypeEnableDisable<status_reg, 8U, reg_access::access_tag::read>;
                /**
                 * @brief The status bit "output signal from selected peripheral, before register override is applied".
                 */
                using outFromPeri_bits = status_reg::template Bits<OutFromPeriField>;
            };

            /**
             * @brief GPIO control register to set the function and overrides.
             */
            struct ctrl {
                static constexpr std::uintptr_t addr = base_addr + ctrlOffset; /**< @brief The address of the gpio's ctrl register. */
                using ctrl_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The gpio control register. */

                /**
                 * @brief The interrupt override bit field
                 */
                struct IRQOverBitField {
                    using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */
                    using bit_access = ctrl_reg::reg_access;

                    static constexpr T position = 0x28U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                    /**
                     * @brief The override values for the interrupt.
                     */
                    enum class value : T {
                        NORMAL = 0x0U, /** @brief Interrupt set to normal. */
                        INVERT = 0x1U, /** @brief Interrupt is inverted. */
                        LOW = 0x02U, /** @brief Interrupt is driven low. */
                        HIGH = 0x3U, /** @brief Interrupt is driven high. */
                    };
                };

                /**
                 * @brief The interrupt override bits.
                 */
                using irqOver_bits = ctrl_reg::template Bits<IRQOverBitField>;

                /**
                 * @brief The input override bit field
                 */
                struct InOverBitField {
                    using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */
                    using bit_access = ctrl_reg::reg_access;

                    static constexpr T position = 0x16U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                    /**
                     * @brief The override values for the input.
                     */
                    enum class value : T {
                        NORMAL = 0x0U, /** @brief Peripheral input set to normal. */
                        INVERT = 0x1U, /** @brief Peripheral input is inverted. */
                        LOW = 0x02U, /** @brief Peripheral input is driven low. */
                        HIGH = 0x3U, /** @brief Peripheral input is driven high. */
                    };
                };

                /**
                 * @brief The input override bits.
                 */
                using inOver_bits = ctrl_reg::template Bits<InOverBitField>;

                /**
                 * @brief The output enable override bit field
                 */
                struct OutEnOverBitField {
                    using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */
                    using bit_access = ctrl_reg::reg_access;

                    static constexpr T position = 0x12U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                    /**
                     * @brief The override values for output enable.
                     */
                    enum class value : T {
                        NORMAL = 0x0U, /** @brief Output enable from peripheral signal selected by funcsel is normal. */
                        INVERT = 0x1U, /** @brief Output enable from peripheral signal selected by funcsel is inverted. */
                        DISABLE = 0x02U, /** @brief Disable output. */
                        ENABLE = 0x3U, /** @brief Enable output. */
                    };
                };

                /**
                 * @brief The output enable override bits.
                 */
                using outEnOver_bits = ctrl_reg::template Bits<OutEnOverBitField>;

                /**
                 * @brief The output override bit field
                 */
                struct OutOverBitField {
                    using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */
                    using bit_access = ctrl_reg::reg_access;

                    static constexpr T position = 0x08U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                    /**
                     * @brief The override values for output.
                     */
                    enum class value : T {
                        NORMAL = 0x0U, /** @brief Output from peripheral signal selected by funcsel is normal. */
                        INVERT = 0x1U, /** @brief Output from peripheral signal selected by funcsel is inverted. */
                        LOW = 0x02U, /** @brief Output is driven low. */
                        HIGH = 0x3U, /** @brief Output is driven high. */
                    };
                };

                /**
                 * @brief The output override bits.
                 */
                using outOver_bits = ctrl_reg::template Bits<OutOverBitField>;

                /**
                 * @brief The function select bit field
                 */
                struct FuncSelectBitField {
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
                 * @brief The function select bits.
                 */
                using funcSelect_bits = ctrl_reg::template Bits<FuncSelectBitField>;
            };
        };

        /**
         * @brief The bits to check the interrupts for an GPIO.
         *
         * @tparam gpio The GPIO to which the interrupt bits belong.
         * @tparam base The base of the interrupt register.
         * @tparam type The type of the interrupt register.
         */
        template <Pins gpio, InterruptBase base, InterruptType type>
        struct InterruptGPIOX_RegMap {
            static_assert(gpio < Pins::number_of_gpio, "Invalid GPIO.");

            static_assert((base == InterruptBase::raw_interrupt && type == InterruptType::raw) ||
                    ((base == InterruptBase::proc0 || base == InterruptBase::proc1 || base == InterruptBase::dormant_wake) &&
                    (type == InterruptType::enable || type == InterruptType::force || type == InterruptType::status)), "Invalid combination of interrupt base and type");

            static constexpr std::uint32_t baseOffset =  static_cast<std::uint32_t>(base) + static_cast<std::uint32_t>(type); /**< @brief The offset of the target register block to the USER_BANK_BASE address. */
            static constexpr std::uint32_t gpioPerRegister = 8U; /**< @brief The maximum number of gpio per interrupt register. */
            static constexpr std::uint32_t bytesPerRegister = 4U; /**< @brief A register is 4 byte wide. */
            static constexpr std::uint32_t gpioRegOffset = (static_cast<std::uint32_t>(gpio) / gpioPerRegister) * bytesPerRegister; /**< @brief The offset of the register to which the gpio belongs from the first interrupt register of the register block. */
            static constexpr std::uint32_t bitsPerGpio = 4U; /**< @brief There are four bits per gpio. */
            static constexpr std::uint32_t gpioBitOffset = (static_cast<std::uint32_t>(gpio) % gpioPerRegister) * bitsPerGpio; /**< @brief The bit offset to the interrupt bits of the gpio. */

            /**
             * @brief The interrupt bits of the gpio.
             */
            struct interrupt {
                static constexpr std::uintptr_t addr = ioBankBaseAddr + baseOffset + gpioRegOffset; /**< @brief The address of the specific interrupt register of the gpio. */
                /**
                 * @brief The interrupt register of the GPIO.
                 *
                 * The read-write access depends on the interrupt type. Raw interrupt registers have mixed R0-WC bits, Status registers
                 * are read-only and enable and force interrupt registers have read-write access.
                 */
                using interrupt_reg = decltype([]() {
                    if constexpr (type == InterruptType::raw) {
                        return std::type_identity_t<rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::mixed_access, std::uint32_t>>();
                    } else if constexpr (type == InterruptType::status) {
                        return std::type_identity_t<rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read, std::uint32_t>>();
                    } else {
                        return std::type_identity_t<rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read_write, std::uint32_t>>();
                    }
                }());



                /**
                 * @brief The edge high bit field of the gpio's interrupt register.
                 *
                 *  The read-write access depends on the interrupt type. Raw interrupt edge high bits are write-clear, status
                 * interrupts edge high bits are read-only and enable and force interrupt edge high bits have read-write access.
                 */
                using edgeHighBitField = decltype([]() {
                    if constexpr (type == InterruptType::raw) {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset + 3U, reg_access::access_tag::write_clear>>();
                    } else if constexpr (type == InterruptType::status) {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset + 3U, reg_access::access_tag::read>>();
                    } else {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset + 3U, reg_access::access_tag::read_write>>();
                    }
                }());
                /**
                 * @brief The edge high bit of the gpio's interrupt register.
                 */
                using edgeHigh_bits = interrupt_reg::template Bits<edgeHighBitField>;


                /**
                 * @brief The edge low bit field of the gpio's interrupt register.
                 *
                 * The read-write access depends on the interrupt type. Raw interrupt edge low bits are write-clear, status
                 * interrupts edge low bits are read-only and enable and force interrupt edge low bits have read-write access.
                 */
                using edgeLowBitField = decltype([]() {
                    if constexpr (type == InterruptType::raw) {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset + 2U, reg_access::access_tag::write_clear>>();
                    } else if constexpr (type == InterruptType::status) {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset + 2U, reg_access::access_tag::read>>();
                    } else {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset + 2U, reg_access::access_tag::read_write>>();
                    }
                }());
                /**
                 * @brief The edge low bit of the gpio's interrupt register.
                 */
                using edgeLow_bits = interrupt_reg::template Bits<edgeLowBitField>;

                /**
                 * @brief The high level interrupt bit field of the gpio.
                 *
                 * The read-write access depends on the interrupt type. Raw interrupt level high bits are write-clear, status
                 * interrupts level high bits are read-only and enable and force interrupt level high bits have read-write access.
                 */
                using levelHighBitField = decltype([]() {
                    if constexpr (type == InterruptType::raw) {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset + 1U, reg_access::access_tag::read>>();
                    } else if constexpr (type == InterruptType::status) {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset + 1U, reg_access::access_tag::read>>();
                    } else {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset + 1U, reg_access::access_tag::read_write>>();
                    }
                }());
                /**
                 * @brief The high level interrupt bit of the gpio.
                 */
                using levelHigh_bits = interrupt_reg::template Bits<levelHighBitField>;
                /**
                 * @brief The low level interrupt bit field of the gpio.
                 *
                 * The read-write access depends on the interrupt type. Raw interrupt level low bits are write-clear, status
                 * interrupts level low bits are read-only and enable and force interrupt level low bits have read-write access.
                 */
                using levelLowBitField = decltype([]() {
                    if constexpr (type == InterruptType::raw) {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset, reg_access::access_tag::read>>();
                    } else if constexpr (type == InterruptType::status) {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset, reg_access::access_tag::read>>();
                    } else {
                        return std::type_identity_t<reg_access::BitTypeEnableDisable<interrupt_reg, gpioBitOffset, reg_access::access_tag::read_write>>();
                    }
                }());
                /**
                 * @brief The low level interrupt bit of the gpio.
                 */
                using levelLow_bits = interrupt_reg::template Bits<levelLowBitField>;
            };
        };

        /**
         * @brief The raw interrupt registers.
         *
         * @tparam gpio The gpio for which the status of the interrupts is to be read/cleared.
         */
        template <Pins gpio>
        using rawInterruptRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::raw_interrupt, InterruptType::raw>;

        /**
         * @brief The enable interrupt registers for processor 0.
         *
         * @tparam gpio The gpio for which the interrupt is to be enabled.
         */
        template <Pins gpio>
        using proc0InterruptEnableRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::proc0, InterruptType::enable>;

        /**
         * @brief The force interrupt registers for processor 0.
         *
         * @tparam gpio The gpio for which the interrupt is to be forced.
         */
        template <Pins gpio>
        using proc0InterruptForceRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::proc0, InterruptType::force>;

        /**
         * @brief The status interrupt registers for processor 0.
         *
         * @tparam gpio The gpio for which the interrupt status is to be read.
         */
        template <Pins gpio>
        using proc0InterruptStatusRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::proc0, InterruptType::status>;

        /**
         * @brief The enable interrupt registers for processor 1.
         *
         * @tparam gpio The gpio for which the interrupt is to be enabled.
         */
        template <Pins gpio>
        using proc1InterruptEnableRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::proc1, InterruptType::enable>;

        /**
         * @brief The force interrupt registers for processor 1.
         *
         * @tparam gpio The gpio for which the interrupt is to be forced.
         */
        template <Pins gpio>
        using proc1InterruptForceRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::proc1, InterruptType::force>;

        /**
         * @brief The status interrupt registers for processor 1.
         *
         * @tparam gpio The gpio for which the interrupt status is to be read.
         */
        template <Pins gpio>
        using proc1InterruptStatusRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::proc1, InterruptType::status>;

        /**
         * @brief The enable interrupt registers for dormant wake.
         *
         * @tparam gpio The gpio for which the interrupt is to be enabled.
         */
        template <Pins gpio>
        using dormWakeInterruptEnableRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::dormant_wake, InterruptType::enable>;

        /**
         * @brief The force interrupt registers for dormant wake.
         *
         * @tparam gpio The gpio for which the interrupt is to be forced.
         */
        template <Pins gpio>
        using dormWakeInterruptForceRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::dormant_wake, InterruptType::force>;

        /**
         * @brief The status interrupt registers for dormant wake.
         *
         * @tparam gpio The gpio for which the interrupt status is to be read.
         */
        template <Pins gpio>
        using dormWakeInterruptStatusRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::dormant_wake, InterruptType::status>;

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
