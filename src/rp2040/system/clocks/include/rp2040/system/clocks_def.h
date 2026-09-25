/**
 * @file clocks_def.h
 * @brief Definitions of the clock control module.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 03.07.2026
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

#ifndef RP2040_SYSTEM_CLOCKS_DEF_H
#define RP2040_SYSTEM_CLOCKS_DEF_H

#include <cstdint>
#include <type_traits>

#include "rp2040/reg_access.h"

namespace rp2040::system::clocks {
    /**
     * @addtogroup rp2040_clocks
     * @{
     */

    /**
     * @brief Checks if the clock type can switch the clock glitchlessly for some clock sources.
     *
     * There are two principal types of clocks in the RP2040. One that has additional src bits, which can be selected
     * glitchlessly, and the other type without the possibility to switch a clock source glitchlessly. Both types have
     * auxiliary sources, which glitch when changed for both types. The way to change the auxiliary source differs
     * between the two types. For the type with the option to glitchlessly change the clock source, another glitchless
     * source is selected to prevent the auxiliary source from glitching and after that the clock source is switched to
     * use the auxiliary source as a primary source. For the other type, there is an option to stop the clock. That is
     * done when switching the auxiliary source, the only type of source for that type and after the switch, the clock is
     * enabled again. This concept serve as to check if the clock has glitchless src bits.
     *
     * @tparam ClockDefType The clock type to be checked.
     */
    template <typename ClockDefType>
    concept ClockHasGlitchlessSrcType = requires {typename ClockDefType::ctrl::template src_bits<0x0U>;};

    /**
     * @brief Checks if the clock type is of the type which only has the glitching aux src register.
     *
     * This type is characterized by having an enable bit in the ctrl register.
     *
     * @tparam ClockDefType The clock type to be checked.
     */
    template <typename ClockDefType>
    concept ClockOnlyGlitchingAuxSrcType = requires {typename ClockDefType::ctrl::enable_bits;};

    /**
     * @brief Describes the minimum requirements for a ClockType.
     *
     * The minimum requirements are:
     * - An enum to select the clock source
     * - A struct to represent the ctrl register
     * - The base address of the clock register
     * - Have either src bits or an enable bit
     *
     * @tparam ClockDefType The type to be checked.
     */
    template <typename ClockDefType>
    concept IsClockType =
        std::is_enum_v<typename ClockDefType::ClockSrc> &&
        std::is_class_v<typename ClockDefType::ctrl> &&
        std::is_same_v<decltype(ClockDefType::base_addr), const std::uintptr_t> &&
        (ClockHasGlitchlessSrcType<ClockDefType> || ClockOnlyGlitchingAuxSrcType<ClockDefType>);


    /**
     * @brief Checks if the clock has a div register with a fractional component.
     *
     * @tparam ClockDefType The type to be checked.
     */
    template <typename ClockDefType>
    concept HasDivWithFracRegister = requires {typename ClockDefType::div::template frac_bits<0x0U>;};

    /**
     * @brief Checks if the clock has a div register.
     *
     * @tparam ClockDefType The type to be checked.
     */
    template <typename ClockDefType>
    concept HasDivRegister = std::is_class_v<typename ClockDefType::div>;

    /**
     * @brief Address of the clock module.
     */
    namespace address {
        constexpr std::uintptr_t clocks_base = 0x40008000U; /**< @brief Base address of clocks. */
        /**
         * @brief Offsets of the different clocks from the base address.
         */
        namespace offset {
            constexpr std::uint32_t gpout0 = 0x00U; /**< @brief */
            constexpr std::uint32_t gpout1 = 0x0CU;
            constexpr std::uint32_t gpout2 = 0x18U;
            constexpr std::uint32_t gpout3 = 0x24U;
            constexpr std::uint32_t ref = 0x30U;
            constexpr std::uint32_t sys = 0x3CU;
            constexpr std::uint32_t peri = 0x48U;
            constexpr std::uint32_t usb = 0x54U;
            constexpr std::uint32_t adc = 0x60U;
            constexpr std::uint32_t rtc = 0x6CU;
        }

        /**
         * @brief Base addresses of the clocks.
         */
        namespace base {
            constexpr std::uintptr_t gpout0 = clocks_base + offset::gpout0;
            constexpr std::uintptr_t gpout1 = clocks_base + offset::gpout1;
            constexpr std::uintptr_t gpout2 = clocks_base + offset::gpout2;
            constexpr std::uintptr_t gpout3 = clocks_base + offset::gpout3;
            constexpr std::uintptr_t ref = clocks_base + offset::ref;
            constexpr std::uintptr_t sys = clocks_base + offset::sys;
            constexpr std::uintptr_t peri = clocks_base + offset::peri;
            constexpr std::uintptr_t usb = clocks_base + offset::usb;
            constexpr std::uintptr_t adc = clocks_base + offset::adc;
            constexpr std::uintptr_t rtc = clocks_base + offset::rtc;
        }
    }



    constexpr std::uintptr_t clocks_base = 0x40008000U; /**< @brief Base address of clocks. */

    constexpr std::uint32_t ref_base_offset = 0x30U; /**< @brief Offset for reference clock control registers. */
    constexpr std::uint32_t sys_base_offset = 0x3CU; /**< @brief Offset for system clock control registers. */
    constexpr std::uint32_t peri_base_offset = 0x48U; /**< @brief Offset for peripheral clock control registers. */

    constexpr std::uintptr_t ref_base = clocks_base + ref_base_offset; /**< @brief The base address of the reference clock registers. */
    constexpr std::uintptr_t sys_base = clocks_base + sys_base_offset; /**< @brief The base address of the system clock registers. */
    constexpr std::uintptr_t peri_base = clocks_base + peri_base_offset; /**< @brief The base address of the peripheral clock registers. */

    constexpr std::uint32_t ctrl_offset = 0x00U; /**< @brief Offset for the control register. */
    constexpr std::uint32_t div_offset = 0x04U; /**< @brief Offset for the divisor register. */
    constexpr std::uint32_t selected_offset = 0x08U; /**< @brief Offset for the selected register. */

    /**
     * @brief Maximum number of bits reserved for the src bits in the ctrl register.
     *
     * The actual src bits of the clocks may use fewer bits.
     */
    constexpr std::uint32_t ctrlSrcMask_max = 0x1FU;

    /**
     * @brief Maximum number of bits reserved for the auxsrc bits in the ctrl register.
     *
     * The actual auxsrc bits of the clocks may use fewer bits.
     */
    constexpr std::uint32_t ctrlAuxSrcMask_max = 0x3E0U;

    /**
     * @brief The auxiliary source bit of the ctrl register.
     *
     * This bit is the same for every clock with src bits.
     */
    constexpr std::uint32_t ctrlSrcAuxSrcBit = (1U<<0U);

    constexpr std::uint32_t ctrlSrcPosition = 0x00U; /**< @brief The Position of the src bits in the ctrl register. */
    constexpr std::uint32_t ctrlAuxSrcPosition = 0x05U; /**< @brief The Position of the auxsrc bits in the ctrl register. */

    /**
     * @brief Checks if a type has the required format to describe the layout of a clock.
     *
     * The clocks don't necessarily use every bit or the amount of bits used varies between clocks, making a single, fixed
     * register map to describe a clock not feasable. A clock layout is used to enable/disable bits (boolean) or set the
     * mask or max value (integer). A field described by an integer can be disabled by setting the mask to 0.
     *
     * A clock layout has the following form:
     *  struct Layout {
     *      struct Ctrl {
     *          static constexpr bool nudge = false;
     *          static constexpr bool phase = false;
     *          static constexpr bool dc50 = false;
     *          static constexpr bool enable = false;
     *          static constexpr bool kill = false;
     *          static constexpr std::uint8_t aux_src_mask = 0x0U;
     *          static constexpr std::uint8_t aux_src_max_val = 0U;
     *          static constexpr std::uint8_t src_mask = 0x0U;
     *          static constexpr std::uint8_t src_max_val = 0U;
     *      };
     *      struct Div {
     *          static constexpr std::uint8_t int_mask = 0x0U;
     *          static constexpr std::uint8_t frac_mask = 0x0U;
     *      };
     *  };
     *
     * @tparam ClockLayout The clock layout candidate.
     */
    template <typename ClockLayout>
    concept ClockLayoutConcept =
        std::is_class_v<typename ClockLayout::Ctrl> &&
        std::is_same_v<typename ClockLayout::Ctrl::nudge, bool> &&
        std::is_same_v<typename ClockLayout::Ctrl::phase, bool> &&
        std::is_same_v<typename ClockLayout::Ctrl::dc50, bool> &&
        std::is_same_v<typename ClockLayout::Ctrl::enable, bool> &&
        std::is_same_v<typename ClockLayout::Ctrl::kill, bool> &&
        std::is_integral_v<typename ClockLayout::Ctrl::aux_src_mask> &&
        std::is_integral_v<typename ClockLayout::Ctrl::aux_src_max_val> &&
        std::is_integral_v<typename ClockLayout::Ctrl::src_mask> &&
        std::is_integral_v<typename ClockLayout::Ctrl::src_max_val> &&
        std::is_class_v<typename ClockLayout::Div> &&
        std::is_integral_v<typename ClockLayout::Div::int_mask> &&
        std::is_integral_v<typename ClockLayout::Div::frac_mask>;


    /**
     * @brief Generic register map of a clock.
     *
     * @tparam base Base address of a clock.
     * @tparam ClockLayout The layout of the clock.
     */
    template <std::uintptr_t base, ClockLayoutConcept ClockLayout>
    struct ClockRegMap {
        static constexpr std::uint32_t ctrl_offset = 0x00U; /**< @brief Offset for the control register. */
        static constexpr std::uint32_t div_offset = 0x04U; /**< @brief Offset for the divisor register. */
        static constexpr std::uint32_t selected_offset = 0x08U; /**< @brief Offset for the selected register. */

        /**
         * @brief Clock control register.
         *
         * The bits (except AuxSrc) can be changed on-the-fly.
         */
        struct Ctrl {
            static constexpr std::uintptr_t addr = base + ctrl_offset; /**< @brief Address of the control register. */
            using ctrl_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The control register. */

            /**
             * @brief Shifts the phase of the output by 1 cycle of the input clock.
             */
            template <typename = std::enable_if_t<ClockLayout::Ctrl::nudge>>
            struct Nudge {
                static constexpr std::uint32_t pos = 20U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<ctrl_reg, pos>; /**< @brief Description of the nudge bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the nudge bit. */
                using Value = BitType::value; /**< @brief Value of the nudge bit. */
            };

            /**
             * @brief Delays the enable signal by up to 3 cycles of the input clock.
             *
             * Bits must be set before the clock is enabled to have an effect.
             *
             * @tparam value The number of cycles.
             */
            template <std::uint32_t value, typename = std::enable_if_t<ClockLayout::Ctrl::phase>>
            struct Phase {
                static constexpr std::uint32_t pos = 16U; /**< @brief Position of the bit in the register. */
                static constexpr std::uint32_t mask = 0x03U; /**< @brief Mask of the bits. */
                using BitType = reg_access::BitTypeValues<value, ctrl_reg, mask, pos>; /**< @brief Description of the phase bits. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the phase bits. */
                using Value = BitType::value; /**< @brief Value of the phase bits. */
            };

            /**
             * @brief Duty cycle correction for odd divisors
             */
            template <typename = std::enable_if_t<ClockLayout::Ctrl::dc50>>
            struct Dc50 {
                static constexpr std::uint32_t pos = 12U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<ctrl_reg, pos>; /**< @brief Description of the dc50 bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the dc50 bit. */
                using Value = BitType::value; /**< @brief Value of the dc50 bit. */
            };

            /**
             * @brief Starts and stops the clock generator cleanly.
             */
            template <typename = std::enable_if_t<ClockLayout::Ctrl::enable>>
            struct Enable {
                static constexpr std::uint32_t pos = 11U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<ctrl_reg, pos>; /**< @brief Description of the enable bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the enable bit. */
                using Value = BitType::value; /**< @brief Value of the enable bit. */
            };

            /**
             * @brief Asynchronously kills the clock generator.
             */
            template <typename = std::enable_if_t<ClockLayout::Ctrl::kill>>
            struct Kill {
                static constexpr std::uint32_t pos = 10U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<ctrl_reg, pos>; /**< @brief Description of the kill bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the kill bit. */
                using Value = BitType::value; /**< @brief Value of the kill bit. */
            };

            /**
             * @brief Selects the auxiliary clock source, will glitch when switching.
             *
             * @tparam Value_ The value of the clock source.
             */
            template <std::uint32_t Value_, typename = std::enable_if_t<(ClockLayout::Ctrl::aux_src_mask > 0)>>
            struct AuxSrc {
                struct  BitType {
                    using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */
                    using bit_access = ctrl_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0x05U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (ClockLayout::Ctrl::aux_src_mask << position); /**< @brief The mask of the bits. */

                    static_assert(Value_ <= ClockLayout::Ctrl::aux_src_max_val, "Invalid value for the auxiliary src.");

                    /**
                     * @brief The values to which the auxiliary clock source can be set.
                     */
                    enum class value : T {
                        val = Value_
                    };
                };
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the auxiliary source bits. */
                using Value = BitType::value; /**< @brief Value of the auxiliary source bits. */
            };

            /**
             * @brief Selects the clock source.
             *
             * Selecting this clock happens glitchlessly and therefore the source can be changed on-the-fly.
             *
             * @tparam Value_ The value of the clock source.
             */
            template <std::uint32_t Value_, typename = std::enable_if_t<(ClockLayout::Ctrl::src_mask > 0)>>
            struct Src {
                struct  BitType {
                    using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */
                    using bit_access = ctrl_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (ClockLayout::Ctrl::src_mask << position); /**< @brief The mask of the bits. */

                    static_assert(Value_ <= ClockLayout::Ctrl::src_max_val, "Invalid value for the src.");

                    /**
                     * @brief The values to which the clock source can be set.
                     */
                    enum class value : T {
                        val = Value_
                    };
                };
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the source bits. */
                using Value = BitType::value; /**< @brief Value of the source bits. */
            };
        };

        /**
         * @brief The clock divisor register.
         *
         * Can be changed on-the-fly.
         */
        struct Div {
            static constexpr std::uintptr_t addr = base + div_offset; /**< @brief Address of the divisor register. */
            using div_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The divisor register. */

            /**
             * @brief Integer bits of the clock divisor.
             *
             * Setting the value to 0 divides the clock by 2^16.
             *
             * @tparam value The value of the integer component.
             */
            template <std::uint32_t value, typename = std::enable_if_t<(ClockLayout::Div::int_mask > 0)>>
            struct Int {
                static constexpr std::uint32_t pos = 8U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeValues<value, div_reg, ClockLayout::Div::int_mask, pos>; /**< @brief Description of the int bits. */
                using Bits = div_reg::template Bits<BitType>; /**< @brief Object to handle the int bits. */
                using Value = BitType::value; /**< @brief Value of the int bits. */
            };

            /**
             * @brief Fractional bits of the clock divisor.
             *
             * @tparam value Value of the fractional component.
             */
            template <std::uint32_t value, typename = std::enable_if_t<(ClockLayout::Div::frac_mask > 0)>>
            struct Frac {
                static constexpr std::uint32_t pos = 0U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeValues<value, div_reg, ClockLayout::Div::frac_mask, pos>; /**< @brief Description of the frac bits. */
                using Bits = div_reg::template Bits<BitType>; /**< @brief Object to handle the frac bits. */
                using Value = BitType::value; /**< @brief Value of the frac bits. */
            };
        };

        /**
         * @brief Indicates which Src is currently selected.
         *
         * This whole register is only used to read the selected Src, therefore there is no point in defining bits. The
         * whole register can be read with the register access function Selected::selected_reg::read().
         *
         * On clocks without Src bits, this register is hardwired to 0x01U.
         */
        struct Selected {
            static constexpr std::uintptr_t addr = base + selected_offset; /**< @brief Address of the selected register. */
            using selected_reg = reg_access::Reg<addr, reg_access::access_tag::read, std::uint32_t>; /**< @brief The selected register. */
        };
    };


    /**
     * @brief Defines the reference clock with its registers.
     *
     */
    struct RefClock_DefType {
        static constexpr std::uintptr_t base_addr = ref_base; /**< @brief The base address of the reference clock registers. */

        /**
         * @brief The clock sources for the reference clock.
         */
        enum class ClockSrc : std::uint32_t {
            ROSC = (0x00U << ctrlSrcPosition), /**< @brief The ring oscillator. */
            XOSC = (0x02U << ctrlSrcPosition), /**< @brief The crystal oscillator */
            PLL_USB = (0x00U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The usb pll. */
            GPIN0 = (0x01U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief External clock provided through GPIN0. */
            GPIN1 = (0x02U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief External clock provided through GPIN1. */
        };

        /**
         * @brief Register to control the reference clock.
         */
        struct ctrl {
            static constexpr std::uintptr_t addr = base_addr + ctrl_offset; /**< @brief The address of the ctrl register of the reference clock control. */
            using ctrl_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The control register of the reference clock. */

            /**
             * @brief The auxiliary clock source bits as a BitField.
             *
             * @tparam Value The value for the auxiliary source for the reference clock.
             */
            template <std::uint32_t Value>
            struct AuxSrcBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write; /**< @brief The access tag defining bit operations. */

                static constexpr T position = 0x05U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0x02U, "Invalid value for the auxiliary src of the reference clock.");

                /**
                 * @brief The values to which the auxiliary clock source can be set.
                 *
                 * Values:
                 * - 0x00 PLL_USB
                 * - 0x01 GPIN0
                 * - 0x02 GPIN1
                 */
                enum class value : T {
                    val = Value
                };
            };

            /**
             * @brief The auxiliary source bits.
             *
             * @tparam Value The value for the auxiliary source for the reference clock.
             */
            template <std::uint32_t Value>
            using aux_src_bits = ctrl_reg::Bits<AuxSrcBitField<Value>>;

            /**
            * @brief The clock source bits as a BitField.
            *
            * @tparam Value The value of the src for the reference clock.
            */
            template <std::uint32_t Value>
            struct SrcBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write;

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0x02U, "Invalid value for the reference clock.");

                /**
                 * @brief The values to which the reference clock source can be set.
                 *
                 * Values:
                 * - 0x00 ROSC
                 * - 0x01 AUXSRC
                 * - 0x02 XOSC
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The src bits.
             *
             * @tparam Value The value of the src for the reference clock.
             */
            template <std::uint32_t Value>
            using src_bits = ctrl_reg::Bits<SrcBitField<Value>>;
        };

        /**
         * @brief The div register of the reference clock.
         */
        struct div {
            static constexpr std::uintptr_t addr = base_addr + div_offset; /**< @brief The addr of the div register of the reference clock. */
            using div_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The div register. */

            /**
             * @brief The integer component of the clock divisor.
             *
             * @tparam Value The value to which the integer part is to be set.
             */
            template <std::uint32_t Value>
            struct IntBitField {
                using reg = div_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write;

                static constexpr T position = 0x08U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0x03U, "Invalid value for the int bits.");

                /**
                 * @brief The value to which the integer component is to be set.
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The int bits of the reference clock.
             *
             * @tparam Value The value to which the integer component is to be set.
             */
            template <std::uint32_t Value>
            using int_bits = div_reg::Bits<IntBitField<Value>>;
        };

        /**
         * @brief The selected register contains information about the selected clock.
         */
        struct selected {
            static constexpr std::uintptr_t addr = base_addr + selected_offset; /**< @brief The addr of the selected register of the reference clock. */
            using selected_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read, std::uint32_t>; /**< @brief The selected register. */

            /**
             * @brief The selected bits contain information about the selected clock in the src bits of the ctrl register.
             */
            struct SelectedBitField {
                using reg = selected_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write;

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0xFFFFFFFFU << position); /**< @brief The mask of the bits. */

                /**
                 * @brief Each clock corresponds to a bit in the selected register.
                 */
                enum class value : T {
                    ROSC = (1U << 0U), /**< @brief The ROSC bit. */
                    AUX = (1U << 1U), /**< @brief The auxiliary source bit. */
                    XOSC = (1U << 2U), /**< @brief The XOSC bit. */
                };
            };

            /**
             * @brief The selected bits of the selected register.
             */
            using selected_bits = selected_reg::Bits<SelectedBitField>;
        };
    };

    /**
     * @brief The system clock.
     */
    struct SysClock_DefType {
        static constexpr std::uintptr_t base_addr = sys_base; /**< @brief Base address of the system clock registers. */

        /**
         * @brief The clock sources for the system clock.
         */
        enum class ClockSrc : std::uint32_t {
            CLK_REF = (0x00U << ctrlSrcPosition), /**< @brief The reference clock as a source for the system clock. @see RefClock_DefType */
            PLL_SYS = (0x00U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The system pll. */
            PLL_USB = (0x01U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The usb pll. */
            ROSC = (0x02U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The ring oscillator. */
            XOSC = (0x03U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The crystal oscillator. */
            GPIN0 = (0x04U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief External clock through GPIN0. */
            GPIN1 = (0x05U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief External clock through GPIN1. */
        };

        /**
         * @brief Register to control the system clock.
         */
        struct ctrl {
            static constexpr std::uintptr_t addr = base_addr + ctrl_offset; /**< @brief The address of the ctrl register of the system clock control. */
            using ctrl_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The control register of the system clock. */

            /**
             * @brief The auxiliary clock source bits as a BitField.
             *
             * @tparam Value The value for the auxiliary source for the system clock.
             */
            template <std::uint32_t Value>
            struct AuxSrcBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write;

                static constexpr T position = 0x05U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x07U << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0x05U, "Invalid value for the auxiliary src of the system clock.");

                /**
                 * @brief The values to which the auxiliary clock source can be set.
                 *
                 * Values:
                 * - 0x00 PLL_SYS
                 * - 0x01 PLL_USB
                 * - 0x02 ROSC
                 * - 0x03 XOSC
                 * - 0x04 GPIN0
                 * - 0x05 GPIN1
                 */
                enum class value : T {
                    val = Value
                };
            };

            /**
             * @brief The auxiliary source bits.
             *
             * @tparam Value The value for the auxiliary source for the system clock.
             */
            template <std::uint32_t Value>
            using aux_src_bits = ctrl_reg::Bits<AuxSrcBitField<Value>>;

            /**
            * @brief The clock source bits as a BitField.
            *
            * @tparam Value The value of the src for the system clock.
            */
            template <std::uint32_t Value>
            struct SrcBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write;

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x03U << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0x01U, "Invalid value for the system clock.");

                /**
                 * @brief The values to which the system clock source can be set.
                 *
                 * Values:
                 * - 0x00 CLK_REF
                 * - 0x01 AUXSRC
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The src bits.
             *
             * @tparam Value The value of the src for the reference clock.
             */
            template <std::uint32_t Value>
            using src_bits = ctrl_reg::Bits<SrcBitField<Value>>;
        };

        /**
         * @brief The register to set the clock divider.
         */
        struct div {
            static constexpr std::uintptr_t addr = base_addr + div_offset; /**< @brief The address of the div register of the system clock. */
            using div_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The div register. */

            /**
             * @brief The integer component of the clock divisor.
             *
             * @tparam Value The value to which the integer part is to be set.
             */
            template <std::uint32_t Value>
            struct IntBitField {
                using reg = div_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write;

                static constexpr T position = 0x08U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0xFFFFFFU << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0xFFFFFFU, "Invalid value for the int bits.");

                /**
                 * @brief The value to which the integer component is to be set.
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The int bits of the system clock.
             *
             * @tparam Value The value to which the integer component is to be set.
             */
            template <std::uint32_t Value>
            using int_bits = div_reg::Bits<IntBitField<Value>>;

            /**
             * @brief The fractional component of the clock divisor.
             *
             * @tparam Value The value to which the fractional part is to be set.
             */
            template <std::uint32_t Value>
            struct FracBitField {
                using reg = div_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write;

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0xFFU << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0xFFU, "Invalid value for the frac bits.");

                /**
                 * @brief The value to which the fractional component is to be set.
                 */
                enum class value : T {
                    val = Value,
                };
            };

            /**
             * @brief The int bits of the system clock.
             *
             * @tparam Value The value to which the integer component is to be set.
             */
            template <std::uint32_t Value>
            using frac_bits = div_reg::Bits<FracBitField<Value>>;

        };


        /**
         * @brief The selected register contains information about the selected clock.
         */
        struct selected {
            static constexpr std::uintptr_t addr = base_addr + selected_offset; /**< @brief The addr of the selected register of the system clock. */
            using selected_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read, std::uint32_t>; /**< @brief The selected register. */

            /**
             * @brief The selected bits contain information about the selected clock in the src bits of the ctrl register.
             */
            struct SelectedBitField {
                using reg = selected_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write;

                static constexpr T position = 0x00U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0xFFFFFFFFU << position); /**< @brief The mask of the bits. */

                /**
                 * @brief Each clock corresponds to a bit in the selected register.
                 */
                enum class value : T {
                    CLK_REF = (1U << 0U), /**< @brief The reference clock bit. */
                    AUX = (1U << 1U), /**< @brief The auxiliary source bit. */
                };
            };

            /**
             * @brief The selected bits of the selected register.
             */
            using selected_bits = selected_reg::Bits<SelectedBitField>;
        };
    };

    /**
     * @brief The peripheral clock.
     */
    struct PeriClock_DefType {
        static constexpr std::uintptr_t base_addr = peri_base; /**< @brief Base address of the peripheral clock registers. */

        /**
         * @brief The clock sources for the peripheral clock.
         */
        enum class ClockSrc : std::uint32_t {
            CLK_SYS = (0x00U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The system clock. @see SysClock */
            PLL_SYS = (0x01U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The system pll. */
            PLL_USB = (0x02U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The usb pll. */
            ROSC = (0x03U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The ring oscillator. */
            XOSC = (0x04U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief The crystal oscillator. */
            GPIN0 = (0x05U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief External clock through GPIN0. */
            GPIN1 = (0x06U << ctrlAuxSrcPosition) | ctrlSrcAuxSrcBit, /**< @brief External clock through GPIN1. */
        };

        /**
         * @brief Register to control the peripheral clock.
         */
        struct ctrl {
            static constexpr std::uintptr_t addr = base_addr + ctrl_offset; /**< @brief The address of the ctrl register of the peripheral clock control. */
            using ctrl_reg = rp2040::reg_access::Reg<addr, rp2040::reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The control register of the peripheral clock. */


            /**
             * @brief The enable bit of the peripheral clock.
             */
            using EnableBitField = rp2040::reg_access::BitTypeEnableDisable<ctrl_reg, 11U>;
            /**
             * @brief The enable bit of the peripheral clock.
             */
            using enable_bits = ctrl_reg::Bits<EnableBitField>;

            /**
             * @brief The kill bit of the peripheral clock.
             */
            using KillBitField = rp2040::reg_access::BitTypeEnableDisable<ctrl_reg, 10U>;
            /**
             * @brief The kill bit of the peripheral clock.
             */
            using kill_bits = ctrl_reg::Bits<KillBitField>;

            /**
             * @brief The auxiliary clock source bits as a BitField.
             *
             * @tparam Value The value for the auxiliary source for the peripheral clock.
             */
            template <std::uint32_t Value>
            struct AuxSrcBitField {
                using reg = ctrl_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write;

                static constexpr T position = 0x05U; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x07U << position); /**< @brief The mask of the bits. */

                static_assert(Value <= 0x06U, "Invalid value for the auxiliary src of the peripheral clock.");

                /**
                 * @brief The values to which the auxiliary clock source can be set.
                 *
                 * Values:
                 * - 0x00 CLK_SYS
                 * - 0x01 PLL_SYS
                 * - 0x02 PLL_USB
                 * - 0x03 ROSC
                 * - 0x04 XOSC
                 * - 0x05 GPIN0
                 * - 0x06 GPIN1
                 */
                enum class value : T {
                    val = Value
                };
            };

            /**
             * @brief The auxiliary source bits.
             *
             * @tparam Value The value for the auxiliary source for the peripheral clock.
             */
            template <std::uint32_t Value>
            using aux_src_bits = ctrl_reg::Bits<AuxSrcBitField<Value>>;
        };
    };

    /** @}*/ //rp2040_clocks
} // rp2040::system::clocks

#endif // RP2040_SYSTEM_CLOCKS_DEF_H
