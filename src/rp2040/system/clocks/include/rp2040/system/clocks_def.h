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

    namespace clk_src_param {
        /**
         * @brief Maximum number of bits reserved for the src bits in the ctrl register.
         *
         * The actual src bits of the clocks may use fewer bits.
         */
        constexpr std::uint32_t ctrl_src_mask_max = 0x1FU;

        /**
         * @brief Maximum number of bits reserved for the auxsrc bits in the ctrl register.
         *
         * The actual auxsrc bits of the clocks may use fewer bits.
         */
        constexpr std::uint32_t ctrl_aux_src_mask_max = 0x3E0U;

        /**
         * @brief The auxiliary source bit of the ctrl register.
         *
         * This bit is the same for every clock with src bits.
         */
        constexpr std::uint32_t ctrl_src_aux_src_val = (1U<<0U);

        constexpr std::uint32_t ctrl_src_position = 0x00U; /**< @brief The Position of the src bits in the ctrl register. */
        constexpr std::uint32_t ctrl_aux_src_position = 0x05U; /**< @brief The Position of the auxsrc bits in the ctrl register. */
    }

    /**
     * @brief Checks if a type has the required format to describe the layout of a clock.
     *
     * The clocks don't necessarily use every bit or the amount of bits used varies between clocks, making a single, fixed
     * register map to describe a clock not feasable. A clock layout is used to enable/disable bits (boolean) or set the
     * mask or max value (integer). A field described by an integer can be disabled by setting the mask to 0.
     *
     * A clock layout has the following form:
     *  struct Layout {
     *      std::uintptr_t addr = 0x00000000U;
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
     *          static constexpr std::uint32_t int_mask = 0x0U;
     *          static constexpr std::uint8_t frac_mask = 0x0U;
     *      };
     *      enum class Src : std::uint32_t {
     *      };
     *  };
     *
     * @tparam ClockLayout The clock layout candidate.
     */
    template <typename ClockLayout>
    concept ClockLayoutConcept =
        std::is_integral_v<decltype(ClockLayout::addr)> &&
        std::is_class_v<typename ClockLayout::Ctrl> &&
        std::is_same_v<decltype(ClockLayout::Ctrl::nudge), const bool> &&
        std::is_same_v<decltype(ClockLayout::Ctrl::phase), const bool> &&
        std::is_same_v<decltype(ClockLayout::Ctrl::dc50), const bool> &&
        std::is_same_v<decltype(ClockLayout::Ctrl::enable), const bool> &&
        std::is_same_v<decltype(ClockLayout::Ctrl::kill), const bool> &&
        std::is_integral_v<decltype(ClockLayout::Ctrl::aux_src_mask)> &&
        std::is_integral_v<decltype(ClockLayout::Ctrl::aux_src_max_val)> &&
        std::is_integral_v<decltype(ClockLayout::Ctrl::src_mask)> &&
        std::is_integral_v<decltype(ClockLayout::Ctrl::src_max_val)> &&
        std::is_class_v<typename ClockLayout::Div> &&
        std::is_integral_v<decltype(ClockLayout::Div::int_mask)> &&
        std::is_integral_v<decltype(ClockLayout::Div::frac_mask)> &&
        std::is_enum_v<typename ClockLayout::Src>;

    /**
     * @brief The clock layouts.
     */
    namespace layout {
        /**
         * @brief Base layout for GPOUT0-3
         *
         * GPOUT0-3 have the same layout, only the address differs.
         *
         * @tparam base_addr Address of the GPOUT clock.
         */
        template <std::uintptr_t base_addr>
        struct GpOutBase {
            static_assert((base_addr == address::base::gpout0) || (base_addr == address::base::gpout1) ||
                (base_addr == address::base::gpout2) || (base_addr == address::base::gpout3), "Invalid base address for GpOutBase!");
            std::uintptr_t addr = base_addr;
            struct Ctrl {
                static constexpr bool nudge {true};
                static constexpr bool phase {true};
                static constexpr bool dc50 {true};
                static constexpr bool enable {true};
                static constexpr bool kill {true};
                static constexpr std::uint8_t aux_src_mask {0xFU};
                static constexpr std::uint8_t aux_src_max_val {10U};
                static constexpr std::uint8_t src_mask {0x0U};
                static constexpr std::uint8_t src_max_val {0U};
            };
            struct Div {
                static constexpr std::uint32_t int_mask {0xFFFFFFU};
                static constexpr std::uint8_t frac_mask {0xFFU};
            };
            enum class Src : std::uint32_t {
                pll_sys = (0x0U << clk_src_param::ctrl_aux_src_position),
                gpin0 = (0x1U << clk_src_param::ctrl_aux_src_position),
                gpin1 = (0x2U << clk_src_param::ctrl_aux_src_position),
                pll_usb = (0x3U << clk_src_param::ctrl_aux_src_position),
                rosc = (0x4U << clk_src_param::ctrl_aux_src_position),
                xosc = (0x5U << clk_src_param::ctrl_aux_src_position),
                clk_sys = (0x6U << clk_src_param::ctrl_aux_src_position),
                clk_usb = (0x7U << clk_src_param::ctrl_aux_src_position),
                clk_adc = (0x8U << clk_src_param::ctrl_aux_src_position),
                clk_rtc = (0x9U << clk_src_param::ctrl_aux_src_position),
                clk_ref = (0xAU << clk_src_param::ctrl_aux_src_position),
            };
        };
        using GpOut0 = GpOutBase<address::base::gpout0>;
        using GpOut1 = GpOutBase<address::base::gpout1>;
        using GpOut2 = GpOutBase<address::base::gpout2>;
        using GpOut3 = GpOutBase<address::base::gpout3>;

        struct Ref {
            std::uintptr_t addr = address::base::ref;
            struct Ctrl {
                static constexpr bool nudge {false};
                static constexpr bool phase {false};
                static constexpr bool dc50 {false};
                static constexpr bool enable {false};
                static constexpr bool kill {false};
                static constexpr std::uint8_t aux_src_mask {0x3U};
                static constexpr std::uint8_t aux_src_max_val {2U};
                static constexpr std::uint8_t src_mask {0x3U};
                static constexpr std::uint8_t src_max_val {2U};
            };
            struct Div {
                static constexpr std::uint32_t int_mask {0x3U};
                static constexpr std::uint8_t frac_mask {0x0U};
            };
            enum class Src : std::uint32_t {
                rosc = (0x0U << clk_src_param::ctrl_src_position),
                aux = (0x1U << clk_src_param::ctrl_src_position),
                xosc = (0x2U << clk_src_param::ctrl_src_position),
                pll_usb = (0x0U << clk_src_param::ctrl_aux_src_position) | aux,
                gpin0 = (0x1U << clk_src_param::ctrl_aux_src_position) | aux,
                gpin1 = (0x2U << clk_src_param::ctrl_aux_src_position) | aux,
            };
        };

        struct Sys {
            std::uintptr_t addr = address::base::sys;
            struct Ctrl {
                static constexpr bool nudge {false};
                static constexpr bool phase {false};
                static constexpr bool dc50 {false};
                static constexpr bool enable {false};
                static constexpr bool kill {false};
                static constexpr std::uint8_t aux_src_mask {0x7U};
                static constexpr std::uint8_t aux_src_max_val {5U};
                static constexpr std::uint8_t src_mask {0x3U};
                static constexpr std::uint8_t src_max_val {1U};
            };
            struct Div {
                static constexpr std::uint32_t int_mask {0xFFFFFFU};
                static constexpr std::uint8_t frac_mask {0xFFU};
            };
            enum class Src : std::uint32_t {
                clk_ref = (0x0U << clk_src_param::ctrl_src_position),
                aux = (0x1U << clk_src_param::ctrl_src_position),
                pll_sys = (0x0U << clk_src_param::ctrl_aux_src_position) | aux,
                pll_usb = (0x1U << clk_src_param::ctrl_aux_src_position) | aux,
                rosc = (0x2U << clk_src_param::ctrl_aux_src_position) | aux,
                xosc = (0x3U << clk_src_param::ctrl_aux_src_position) | aux,
                gpin0 = (0x4U << clk_src_param::ctrl_aux_src_position) | aux,
                gpin1 = (0x5U << clk_src_param::ctrl_aux_src_position) | aux,
            };
        };

        struct Peri {
            std::uintptr_t addr = address::base::peri;
            struct Ctrl {
                static constexpr bool nudge {false};
                static constexpr bool phase {false};
                static constexpr bool dc50 {false};
                static constexpr bool enable {true};
                static constexpr bool kill {true};
                static constexpr std::uint8_t aux_src_mask {0x7U};
                static constexpr std::uint8_t aux_src_max_val {6U};
                static constexpr std::uint8_t src_mask {0x0U};
                static constexpr std::uint8_t src_max_val {0U};
            };
            struct Div {
                static constexpr std::uint32_t int_mask {0x0U};
                static constexpr std::uint8_t frac_mask {0x0U};
            };
            enum class Src : std::uint32_t {
                clk_sys = (0x0U << clk_src_param::ctrl_aux_src_position),
                pll_sys = (0x1U << clk_src_param::ctrl_aux_src_position),
                pll_usb = (0x2U << clk_src_param::ctrl_aux_src_position),
                rosc = (0x3U << clk_src_param::ctrl_aux_src_position),
                xosc = (0x4U << clk_src_param::ctrl_aux_src_position),
                gpin0 = (0x5U << clk_src_param::ctrl_aux_src_position),
                gpin1 = (0x6U << clk_src_param::ctrl_aux_src_position),
            };
        };
    }


    /**
     * @brief Generic register map of a clock.
     *
     * @tparam ClockLayout The layout of the clock.
     */
    template <ClockLayoutConcept ClockLayout>
    struct ClockRegMap {
        static_assert((ClockLayout::addr == address::base::gpout0) || (ClockLayout::addr == address::base::gpout1) ||
            (ClockLayout::addr == address::base::gpout2) || (ClockLayout::addr == address::base::gpout3) ||
            (ClockLayout::addr == address::base::ref) || (ClockLayout::addr == address::base::sys) ||
            (ClockLayout::addr == address::base::peri) || (ClockLayout::addr == address::base::usb) ||
            (ClockLayout::addr == address::base::adc) || (ClockLayout::addr == address::base::rtc),
            "Invalid base address for clock regmap!");

        static constexpr std::uint32_t ctrl_offset = 0x00U; /**< @brief Offset for the control register. */
        static constexpr std::uint32_t div_offset = 0x04U; /**< @brief Offset for the divisor register. */
        static constexpr std::uint32_t selected_offset = 0x08U; /**< @brief Offset for the selected register. */

        /**
         * @brief Clock control register.
         *
         * The bits (except AuxSrc) can be changed on-the-fly.
         */
        struct Ctrl {
            static constexpr std::uintptr_t addr = ClockLayout::addr + ctrl_offset; /**< @brief Address of the control register. */
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
            static constexpr std::uintptr_t addr = ClockLayout::addr + div_offset; /**< @brief Address of the divisor register. */
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
            static constexpr std::uintptr_t addr = ClockLayout::addr + selected_offset; /**< @brief Address of the selected register. */
            using selected_reg = reg_access::Reg<addr, reg_access::access_tag::read, std::uint32_t>; /**< @brief The selected register. */
        };
    };

    /** @}*/ //rp2040_clocks
} // rp2040::system::clocks

#endif // RP2040_SYSTEM_CLOCKS_DEF_H
