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

#include <complex.h>
#include <cstdint>
#include "utils/reg_access.h"

namespace rp2040::system::gpio {
    /**
     * @addtogroup rp2040_gpio
     * @{
     */

    constexpr std::uintptr_t IO_BANK0_BASE = 0x40014000U; /**< @brief Base address of the user bank IO registers. */
    constexpr std::uintptr_t PADS_BANK0_BASE = 0x4001C000U; /**< @brief Base address of the user bank pad control registers. */
    constexpr std::uintptr_t SIO_BASE = 0xD0000000U; /**< @brief Base address of the software input/output registers. */

    /**
     * @brief The GPIOs of the RP2040.
     */
    enum class GPIO : std::uint8_t {
        GPIO0 = 0U,
        GPIO1 = 1U,
        GPIO2 = 2U,
        GPIO3 = 3U,
        GPIO4 = 4U,
        GPIO5 = 5U,
        GPIO6 = 6U,
        GPIO7 = 7U,
        GPIO8 = 8U,
        GPIO9 = 9U,
        GPIO10 = 10U,
        GPIO11 = 11U,
        GPIO12 = 12U,
        GPIO13 = 13U,
        GPIO14 = 14U,
        GPIO15 = 15U,
        GPIO16 = 16U,
        GPIO17 = 17U,
        GPIO18 = 18U,
        GPIO19 = 19U,
        GPIO20 = 20U,
        GPIO21 = 21U,
        GPIO22 = 22U,
        GPIO23 = 23U,
        GPIO24 = 24U,
        GPIO25 = 25U,
        GPIO26 = 26U,
        GPIO27 = 27U,
        GPIO28 = 28U,
        GPIO29 = 29U,
        NumberOfGPIOs
    };

    /**
     * @brief The elements that can be configured via user bank pad control registers.
     */
    enum class Pads : std::uint8_t {
        GPIO0 = 0U,
        GPIO1 = 1U,
        GPIO2 = 2U,
        GPIO3 = 3U,
        GPIO4 = 4U,
        GPIO5 = 5U,
        GPIO6 = 6U,
        GPIO7 = 7U,
        GPIO8 = 8U,
        GPIO9 = 9U,
        GPIO10 = 10U,
        GPIO11 = 11U,
        GPIO12 = 12U,
        GPIO13 = 13U,
        GPIO14 = 14U,
        GPIO15 = 15U,
        GPIO16 = 16U,
        GPIO17 = 17U,
        GPIO18 = 18U,
        GPIO19 = 19U,
        GPIO20 = 20U,
        GPIO21 = 21U,
        GPIO22 = 22U,
        GPIO23 = 23U,
        GPIO24 = 24U,
        GPIO25 = 25U,
        GPIO26 = 26U,
        GPIO27 = 27U,
        GPIO28 = 28U,
        GPIO29 = 29U,
        SWCLK = 30U,
        SWD = 31U,
        NumberOfPads
    };


    /**
     * @brief Available functions for GPIOs without clock functionality.
     *
     * These functions are available for GPIO0 - GPIO19 and GPIO26 - GPIO29.
     */
    enum class GPIO_Functions : std::uint32_t {
        SPI = 1U,
        UART = 2U,
        I2C = 3U,
        PWM = 4U,
        SIO = 5U,
        PIO0 = 6U,
        PIO1 = 7U,
        USB = 9U,
    };

    /**
     * @brief Available functions for GPIOs with clock functionality.
     *
     * These functions are available for GPIO20 - GPIO25
     */
    enum class GPIO_FunctionsWithClock : std::uint32_t {
        SPI = 1U,
        UART = 2U,
        I2C = 3U,
        PWM = 4U,
        SIO = 5U,
        PIO0 = 6U,
        PIO1 = 7U,
        CLOCK = 8U,
        USB = 9U,
    };

    /**
 * @brief The base of the interrupt register.
 */
    enum class InterruptBase : std::uint32_t {
        RAW_INTERRUPT = 0x0F0U,
        PROC0 = 0x100U,
        PROC1 = 0x130U,
        DORMANT_WAKE = 0x160U,
    };

    /**
     * @brief The different interrupt register types.
     */
    enum class InterruptType : std::uint32_t {
        RAW = 0x00U,
        ENABLE = 0x00U,
        FORCE = 0x10U,
        STATUS = 0x20U,
    };

    /**
     * @brief Register map for user bank IO registers.
     *
     * @tparam ioBankBaseAddr The base address of the user bank IO registers.
     */
    template <std::uintptr_t ioBankBaseAddr>
    struct IO_BankRegMap {
        static_assert(ioBankBaseAddr == IO_BANK0_BASE, "Invalid base address for IO_BankRegMap.");

        /**
         * @brief General register map to configure and read the status of a GPIO of the user bank IO.
         *
         * @tparam gpio The GPIO to which the regmap belongs.
         */
        template <GPIO gpio>
        struct GPIOX_RegMap {
            static_assert(gpio < GPIO::NumberOfGPIOs, "Invalid GPIO. There are only 29 user GPIOs.");

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
                using status_reg = utils::reg_access::Reg<addr, utils::reg_access::read_access, std::uint32_t>; /**< @brief The status register. */

                /**
                 * @brief Bitfield for the status bit "interrupt to processors, after override is applied".
                 */
                using IRQToProcBitfield = utils::reg_access::BitFieldEnableDisable<status_reg, 26U>;
                /**
                 * @brief The status bit "interrupt to processors, after override is applied".
                 */
                using irqToProc_bits = status_reg::template Bits<IRQToProcBitfield, utils::reg_access::read_access>;

                /**
                 * @brief Bitfield for the status bit "interrupt from pad before override is applied".
                 */
                using IRQFromPadBitField = utils::reg_access::BitFieldEnableDisable<status_reg, 24U>;
                /**
                 * @brief The status bit "interrupt from pad before override is applied".
                 */
                using irqFromPad_bits = status_reg::template Bits<IRQFromPadBitField, utils::reg_access::read_access>;

                /**
                 * @brief Bitfield for the status bit "input signal to peripheral, after override is applied".
                 */
                using IntToPeriBitField = utils::reg_access::BitFieldEnableDisable<status_reg, 19U>;
                /**
                 * @brief The status bit "input signal to peripheral, after override is applied".
                 */
                using intToPeri_bits = status_reg::template Bits<IntToPeriBitField, utils::reg_access::read_access>;

                /**
                 * @brief Bitfield for the status bit "input signal from pad, before override is applied".
                 */
                using IntFromPadBitField = utils::reg_access::BitFieldEnableDisable<status_reg, 17U>;
                /**
                 * @brief The status bit "input signal from pad, before override is applied".
                 */
                using intFromPad_bits = status_reg::template Bits<IntFromPadBitField, utils::reg_access::read_access>;

                /**
                 * @brief Bitfield for the status bit "output enable to pad after register override is applied".
                 */
                using OEToPadBitField = utils::reg_access::BitFieldEnableDisable<status_reg, 13U>;
                /**
                 * @brief The status bit "output enable to pad after register override is applied".
                 */
                using oeToPad_bits = status_reg::template Bits<OEToPadBitField, utils::reg_access::read_access>;

                /**
                 * @brief Bitfield for the status bit "output enable from selected peripheral, before register override is applied".
                 */
                using OEFromPeriBitField = utils::reg_access::BitFieldEnableDisable<status_reg, 12U>;
                /**
                 * @brief The status bit "output enable from selected peripheral, before register override is applied".
                 */
                using oeFromPeri_bits = status_reg::template Bits<OEFromPeriBitField, utils::reg_access::read_access>;

                /**
                 * @brief Bitfield for the status bit "output signal to pad after register override is applied".
                 */
                using OutToPadBitField = utils::reg_access::BitFieldEnableDisable<status_reg, 9U>;
                /**
                 * @brief The status bit "output signal to pad after register override is applied".
                 */
                using outToPad_bits = status_reg::template Bits<OutToPadBitField, utils::reg_access::read_access>;

                /**
                 * @brief Bitfield for the status bit "output signal from selected peripheral, before register override is applied".
                 */
                using OutFromPeriField = utils::reg_access::BitFieldEnableDisable<status_reg, 8U>;
                /**
                 * @brief The status bit "output signal from selected peripheral, before register override is applied".
                 */
                using outFromPeri_bits = status_reg::template Bits<OutFromPeriField, utils::reg_access::read_access>;
            };

            /**
             * @brief GPIO control register to set the function and overrides.
             */
            struct ctrl {
                static constexpr std::uintptr_t addr = base_addr + ctrlOffset; /**< @brief The address of the gpio's ctrl register. */
                using ctrl_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The gpio control register. */

                /**
                 * @brief The interrupt override bit field
                 */
                struct IRQOverBitField {
                    using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */

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

                    static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (0x1FU << position); /**< @brief The mask of the bits. */

                    /**
                     * @brief The functions the GPIO can be configured for.
                     *
                     * GPIO20 - GPIO25 can additionally be configured for clock input/output.
                     */
                    using value = decltype([]() {
                        if constexpr ((gpio >= GPIO::GPIO20) && (gpio <= GPIO::GPIO25)) {
                            return std::type_identity_t<GPIO_FunctionsWithClock>();
                        } else {
                            return std::type_identity_t<GPIO_Functions>();
                        }
                    }());
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
        template <GPIO gpio, InterruptBase base, InterruptType type>
        struct InterruptGPIOX_RegMap {
            static_assert(gpio < GPIO::NumberOfGPIOs, "Invalid GPIO.");

            static_assert((base == InterruptBase::RAW_INTERRUPT && type == InterruptType::RAW) ||
                    ((base == InterruptBase::PROC0 || base == InterruptBase::PROC1 || base == InterruptBase::DORMANT_WAKE) &&
                    (type == InterruptType::ENABLE || type == InterruptType::FORCE || type == InterruptType::STATUS)), "Invalid combination of interrupt base and type");

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
                    if constexpr (type == InterruptType::RAW) {
                        return std::type_identity_t<utils::reg_access::Reg<addr, utils::reg_access::reg_mixed_access, std::uint32_t>>();
                    } else if constexpr (type == InterruptType::STATUS) {
                        return std::type_identity_t<utils::reg_access::Reg<addr, utils::reg_access::read_access, std::uint32_t>>();
                    } else {
                        return std::type_identity_t<utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>>();
                    }
                }());



                /**
                 * @brief The edge high bit field of the gpio's interrupt register.
                 */
                using edgeHighBitField = utils::reg_access::BitFieldEnableDisable<interrupt_reg, gpioBitOffset + 3U>;
                /**
                 * @brief The edge high bit of the gpio's interrupt register.
                 *
                 * The read-write access depends on the interrupt type. Raw interrupt edge high bits are write-clear, status
                 * interrupts edge high bits are read-only and enable and force interrupt edge high bits have read-write access.
                 */
                using edgeHigh_bits = decltype([]() {
                    if constexpr (type == InterruptType::RAW) {
                        return std::type_identity_t<typename interrupt_reg::template Bits<edgeHighBitField, utils::reg_access::bit_write_clear>>();
                    } else if constexpr (type == InterruptType::STATUS) {
                        return std::type_identity_t<typename interrupt_reg::template Bits<edgeHighBitField, utils::reg_access::read_access>>();
                    } else {
                        return std::type_identity_t<typename interrupt_reg::template Bits<edgeHighBitField, utils::reg_access::read_write_access>>();
                    }
                }());



                /**
                 * @brief The edge low bit field of the gpio's interrupt register.
                 */
                using edgeLowBitField = utils::reg_access::BitFieldEnableDisable<interrupt_reg, gpioBitOffset + 2U>;
                /**
                 * @brief The edge low bit of the gpio's interrupt register.
                 *
                 * The read-write access depends on the interrupt type. Raw interrupt edge low bits are write-clear, status
                 * interrupts edge low bits are read-only and enable and force interrupt edge low bits have read-write access.
                 */
                using edgeLow_bits = decltype([]() {
                    if constexpr (type == InterruptType::RAW) {
                        return std::type_identity_t<typename interrupt_reg::template Bits<edgeLowBitField, utils::reg_access::bit_write_clear>>();
                    } else if constexpr (type == InterruptType::STATUS) {
                        return std::type_identity_t<typename interrupt_reg::template Bits<edgeLowBitField, utils::reg_access::read_access>>();
                    } else {
                        return std::type_identity_t<typename interrupt_reg::template Bits<edgeLowBitField, utils::reg_access::read_write_access>>();
                    }
                }());

                /**
                 * @brief The high level interrupt bit field of the gpio.
                 */
                using levelHighBitField = utils::reg_access::BitFieldEnableDisable<interrupt_reg, gpioBitOffset + 1U>;
                /**
                 * @brief The high level interrupt bit of the gpio.
                 *
                 * The read-write access depends on the interrupt type. Raw interrupt level high bits are write-clear, status
                 * interrupts level high bits are read-only and enable and force interrupt level high bits have read-write access.
                 */
                using levelHigh_bits = decltype([]() {
                    if constexpr (type == InterruptType::RAW) {
                        return std::type_identity_t<typename interrupt_reg::template Bits<levelHighBitField, utils::reg_access::read_access>>();
                    } else if constexpr (type == InterruptType::STATUS) {
                        return std::type_identity_t<typename interrupt_reg::template Bits<levelHighBitField, utils::reg_access::read_access>>();
                    } else {
                        return std::type_identity_t<typename interrupt_reg::template Bits<levelHighBitField, utils::reg_access::read_write_access>>();
                    }
                }());

                /**
                 * @brief The low level interrupt bit field of the gpio.
                 */
                using levelLowBitField = utils::reg_access::BitFieldEnableDisable<interrupt_reg, gpioBitOffset>;
                /**
                 * @brief The low level interrupt bit of the gpio.
                 *
                 * The read-write access depends on the interrupt type. Raw interrupt level low bits are write-clear, status
                 * interrupts level low bits are read-only and enable and force interrupt level low bits have read-write access.
                 */
                using levelLow_bits = decltype([]() {
                    if constexpr (type == InterruptType::RAW) {
                        return std::type_identity_t<typename interrupt_reg::template Bits<levelLowBitField, utils::reg_access::read_access>>();
                    } else if constexpr (type == InterruptType::STATUS) {
                        return std::type_identity_t<typename interrupt_reg::template Bits<levelLowBitField, utils::reg_access::read_access>>();
                    } else {
                        return std::type_identity_t<typename interrupt_reg::template Bits<levelLowBitField, utils::reg_access::read_write_access>>();
                    }
                }());
            };
        };

        /**
         * @brief The raw interrupt registers.
         *
         * @tparam gpio The gpio for which the status of the interrupts is to be read/cleared.
         */
        template <GPIO gpio>
        using rawInterruptRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::RAW_INTERRUPT, InterruptType::RAW>;

        /**
         * @brief The enable interrupt registers for processor 0.
         *
         * @tparam gpio The gpio for which the interrupt is to be enabled.
         */
        template <GPIO gpio>
        using proc0InterruptEnableRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::PROC0, InterruptType::ENABLE>;

        /**
         * @brief The force interrupt registers for processor 0.
         *
         * @tparam gpio The gpio for which the interrupt is to be forced.
         */
        template <GPIO gpio>
        using proc0InterruptForceRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::PROC0, InterruptType::FORCE>;

        /**
         * @brief The status interrupt registers for processor 0.
         *
         * @tparam gpio The gpio for which the interrupt status is to be read.
         */
        template <GPIO gpio>
        using proc0InterruptStatusRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::PROC0, InterruptType::STATUS>;

        /**
         * @brief The enable interrupt registers for processor 1.
         *
         * @tparam gpio The gpio for which the interrupt is to be enabled.
         */
        template <GPIO gpio>
        using proc1InterruptEnableRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::PROC1, InterruptType::ENABLE>;

        /**
         * @brief The force interrupt registers for processor 1.
         *
         * @tparam gpio The gpio for which the interrupt is to be forced.
         */
        template <GPIO gpio>
        using proc1InterruptForceRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::PROC1, InterruptType::FORCE>;

        /**
         * @brief The status interrupt registers for processor 1.
         *
         * @tparam gpio The gpio for which the interrupt status is to be read.
         */
        template <GPIO gpio>
        using proc1InterruptStatusRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::PROC1, InterruptType::STATUS>;

        /**
         * @brief The enable interrupt registers for dormant wake.
         *
         * @tparam gpio The gpio for which the interrupt is to be enabled.
         */
        template <GPIO gpio>
        using dormWakeInterruptEnableRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::DORMANT_WAKE, InterruptType::ENABLE>;

        /**
         * @brief The force interrupt registers for dormant wake.
         *
         * @tparam gpio The gpio for which the interrupt is to be forced.
         */
        template <GPIO gpio>
        using dormWakeInterruptForceRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::DORMANT_WAKE, InterruptType::FORCE>;

        /**
         * @brief The status interrupt registers for dormant wake.
         *
         * @tparam gpio The gpio for which the interrupt status is to be read.
         */
        template <GPIO gpio>
        using dormWakeInterruptStatusRegs = InterruptGPIOX_RegMap<gpio, InterruptBase::DORMANT_WAKE, InterruptType::STATUS>;

    };

    /**
     * @brief The user bank pad control registers.
     *
     * @tparam PadsBaseAddr The base address of the user bank pad control registers.
     */
    template <std::uintptr_t PadsBaseAddr>
    struct PadsBankRegMap {
        static_assert(PadsBaseAddr == PADS_BANK0_BASE, "Invalid address for PadsBaseAddr.");

        static constexpr std::uint8_t voltageSelectOffset = 0x00U; /**< @brief The offset of the voltage select register. */
        static constexpr std::uint8_t padRegisterStartOffset = 0x04U; /**< @brief The offset of the first pad control register. */

        /**
         * @brief The voltage select register.
         */
        struct voltage_select {
            static constexpr std::uintptr_t addr = PadsBaseAddr + voltageSelectOffset; /**< @brief Base address of the voltage select register. */
            using voltage_select_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The voltage select register. */

            /**
             * @brief The voltage select bits as a bit field.
             */
            struct VoltageSelectBitField {
                using reg = voltage_select_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

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
            static_assert(pad < Pads::NumberOfPads, "Invalid Pad. For valid options see enum Pads.");

            static constexpr std::uint8_t bytesPerRegister = 0x04U; /**< @brief The RP2040 is a 32 bit architecture and every register is therefore 4 bytes. */
            static constexpr std::uint8_t padOffset = padRegisterStartOffset + static_cast<std::uint8_t>(pad) * bytesPerRegister; /**< @brief The offset of the specified pad from the first pad (GPIO0). */

            static constexpr std::uintptr_t addr = PadsBaseAddr + padOffset; /**< @brief The address of the pad register to be configured. */
            using ctrl_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The pad register to be configured. */

            /**
             * @brief The output disable bit as a bit field.
             *
             * Disabling the output via this bit has priority over output enable from peripherals.
             */
            struct OutputDisableBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

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
            using InputEnableBitField = utils::reg_access::BitFieldEnableDisable<ctrl_reg, 0x6U>;
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
            using PullUpEnableBitField = utils::reg_access::BitFieldEnableDisable<ctrl_reg, 0x3U>;
            /**
             * @brief The pull up enable bit.
             */
            using pullUpEnable_bits = ctrl_reg::template Bits<PullUpEnableBitField>;

            /**
             * @brief The pull down enable bit as a bit field.
             */
            using PullDownEnableBitField = utils::reg_access::BitFieldEnableDisable<ctrl_reg, 0x2U>;
            /**
             * @brief The pull down enable bit.
             */
            using pullDownEnable_bits = ctrl_reg::template Bits<PullDownEnableBitField>;

            /**
             * @brief The schmitt bit as a bit field to enable/disable the schmitt trigger.
             */
            using SchmittBitField = utils::reg_access::BitFieldEnableDisable<ctrl_reg, 0x1U>;
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
        static_assert(SIOBaseAddr == SIO_BASE, "Invalid address for SIOBaseAddr.");

        static constexpr std::uint32_t cpuidOffset = 0x000U; /**< @brief Offset of the cpuid register to the sio base address. */
        static constexpr std::uint32_t gpioInOffset = 0x004U; /**< @brief Offset of the gpio_in register to the sio base address. */
        static constexpr std::uint32_t gpioOutOffset = 0x010U; /**< @brief Offset of the gpio_out register to the sio base address. */
        static constexpr std::uint32_t gpioOutEnOffset = 0x020U; /**< @brief Offset of the gpio_oe register to the sio base address. */

        /**
         * @brief The processor core identifier register.
         */
        struct cpuid {
            static constexpr std::uintptr_t addr = SIOBaseAddr + cpuidOffset; /**< @brief The base address of the cpuid register. */
            using cpuid_reg = utils::reg_access::Reg<addr, utils::reg_access::read_access, std::uint32_t>; /**< @brief The the cpuid register. */

            /**
             * @brief The processor core identifier bits as a bit field.
             */
            struct CPUIDBitField {
                using reg = cpuid_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

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
            using input_reg = utils::reg_access::Reg<addr, utils::reg_access::read_access, std::uint32_t>; /**< @brief The sio input register. */

            /**
             * @brief The gpio input bit as a bit field.
             *
             * @tparam gpio The GPIO where the input value is to be get.
             */
            template <GPIO gpio>
            requires (gpio < GPIO::NumberOfGPIOs)
            using InputBitField = utils::reg_access::BitFieldHighLow<input_reg, static_cast<input_reg::RegType>(gpio)>;
            /**
             * @brief The input bit of the gpio.
             *
             * @tparam gpio The GPIO where the input value is to be get.
             */
            template <GPIO gpio>
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
             * @brief The output register.
             */
            struct output {
                using output_reg = utils::reg_access::Reg<outputBaseAddr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The output or output enable register. */

                /**
                 * @brief The output/output enable bits as a Bitfield.
                 *
                 * The options only differ in the enum values. High/Low is more reasonable for the output value, while
                 * Enable/Disable is more reasonable to set output enable bits.
                 *
                 * @tparam gpio The gpio to be configured.
                 */
                template <GPIO gpio>
                using OutputBitField = decltype([] () {
                    static_assert(gpio < GPIO::NumberOfGPIOs);
                    if constexpr(baseOffset == gpioOutOffset) {
                        return std::type_identity_t<utils::reg_access::BitFieldHighLow<output_reg, static_cast<output_reg::RegType>(gpio)>>();
                    } else if constexpr (baseOffset == gpioOutEnOffset) {
                        return std::type_identity_t<utils::reg_access::BitFieldEnableDisable<output_reg, static_cast<output_reg::RegType>(gpio)>>();
                    }
                }());

                /**
                 * @brief The output/output enable bits.
                 *
                 * @tparam gpio The gpio to be configured.
                 */
                template <GPIO gpio>
                using output_bits = output_reg::template Bits<OutputBitField<gpio>>;
            };

            /**
             * @brief The output set register.
             *
             * This is an atomic register.
             */
            struct output_set {
                static constexpr std::uintptr_t outputSetAddr = outputBaseAddr + setOffset; /**< @brief Base address of the atomic set register. */
                using outputSet_reg = utils::reg_access::Reg<outputSetAddr, utils::reg_access::atomic_set, std::uint32_t>; /**< @brief The atomic set register. */

                /**
                 * @brief The output set bits.
                 *
                 * This is an atomic operation.
                 *
                 * @tparam gpio The gpio for which the output is to be set.
                 */
                template <GPIO gpio>
                using outputSet_bits = outputSet_reg::template Bits<utils::reg_access::BitFieldAtomicBitOp<outputSet_reg, static_cast<outputSet_reg::RegType>(gpio)>>;
            };

            /**
             * @brief The output clear register.
             *
             * This is an atomic register.
             */
            struct output_clear {
                static constexpr std::uintptr_t outputClearAddr = outputBaseAddr + clearOffset; /**< @brief Base address of the atomic clear register. */
                using outputClear_reg = utils::reg_access::Reg<outputClearAddr, utils::reg_access::atomic_clear, std::uint32_t>; /**< @brief The atomic clear register. */

                /**
                 * @brief The output clear bits.
                 *
                 * This is an atomic operation.
                 *
                 * @tparam gpio The gpio for which the output is to be cleared.
                 */
                template <GPIO gpio>
                using outputClear_bits = outputClear_reg::template Bits<utils::reg_access::BitFieldAtomicBitOp<outputClear_reg, static_cast<outputClear_reg::RegType>(gpio)>>;
            };

            /**
            * @brief The output toggle register.
            *
            * This is an atomic register.
            */
            struct output_toggle {
                static constexpr std::uintptr_t outputXorAddr = outputBaseAddr + xorOffset; /**< @brief Base address of the atomic xor register. */
                using outputToggle_reg = utils::reg_access::Reg<outputXorAddr, utils::reg_access::atomic_xor, std::uint32_t>; /**< @brief The atomic xor register. */

                /**
                 * @brief The output toggle bits.
                 *
                 * This is an atomic operation.
                 *
                 * @tparam gpio The gpio for which the output is to be toggled.
                 */
                template <GPIO gpio>
                using outputClear_bits = outputToggle_reg::template Bits<utils::reg_access::BitFieldAtomicBitOp<outputToggle_reg, static_cast<outputToggle_reg::RegType>(gpio)>>;
            };
        };

        using output = OutputRegMap<gpioOutOffset>; /**< @brief The output registers. */
        using outputEnable = OutputRegMap<gpioOutEnOffset>; /**< @brief The output enable registers. */
    };

    /** @} */ // rp2040_gpio
} // rp2040::system::gpio

#endif //RP2040_SYSTEM_GPIO_DEF_H
