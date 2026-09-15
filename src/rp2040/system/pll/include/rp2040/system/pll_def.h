/**
 * @file pll_def.h
 * @brief Definitions for the phase-locked loop (PLL).
 * @author Thorsten Hoffmeister
 * @version 1.0
 * @date 19.06.2026
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

#ifndef RP2040_SYSTEM_PLL_DEF_H
#define RP2040_SYSTEM_PLL_DEF_H

#include <cstdint>

#include "rp2040/reg_access.h"

namespace rp2040::system::pll {
    /**
     * @addtogroup rp2040_pll
     * @{
     */

    /**
     * @brief Base addresses of the pll.
     */
    namespace address {
        constexpr std::uintptr_t pll_sys = 0x40028000U; /**< @brief The address of the system pll. */
        constexpr std::uintptr_t pll_usb = 0x4002c000U; /**< @brief The address of the usb pll. */
    }


    /**
     * @brief The limits for pll configuration parameters.
     */
    namespace limits {
        constexpr std::uint32_t ref_clock_min_freq_hz = 5'000'000U; /**< @brief Minimum value for frequency of the reference clock. */
        constexpr std::uint32_t ref_clock_max_freq_hz = 800'000'000U; /**< @brief Maximum value for frequency of the reference clock. */
        constexpr std::uint32_t fbdiv_min_val = 16U; /**< @brief The minimum value for the feedback divisor. */
        constexpr std::uint32_t fbdiv_max_val = 320U; /**< @brief The maximum value for the feedback divisor. */
        constexpr std::uint32_t vco_min_freq_hz = 750'000'000U; /**< @brief The minimum value for the frequency of the Voltage Controlled Oscillator (VCO). */
        constexpr std::uint32_t vco_max_freq_hz = 1'600'000'000U;  /**< @brief The maximum value for the frequency of the Voltage Controlled Oscillator (VCO). */
        constexpr std::uint8_t postdiv_min_val = 1U; /**< @brief The minimum value for the post divider. */
        constexpr std::uint8_t postdiv_max_val = 7U; /**< @brief The maximum value for the post divider. */
        constexpr std::uint32_t pll_sys_max_freq_hz = 133'000'000U; /**< @brief The maximum value for the RP2040's system PLL. */
        constexpr std::uint32_t pll_usb_freq_hz = 48'000'000U; /**< @brief The value for the RP2040's USB PLL. */
    }

    /**
     * @brief The register map of the pll
     *
     * @tparam pll_addr The address of the pll.
     */
    template <std::uintptr_t pll_addr>
    struct PllRegMapType {
        static_assert((pll_addr == address::pll_sys) || (pll_addr == address::pll_usb), "Invalid pll address.");

        static constexpr std::uint32_t cs_offset = 0x00U; /**< @brief The offset of the control and status register. */
        static constexpr std::uint32_t pwr_offset = 0x04U; /**< @brief The offset of the power register. */
        static constexpr std::uint32_t fbdiv_int_offset = 0x08U; /**< @brief The offset of the feedback divisor register. */
        static constexpr std::uint32_t prim_offset = 0x0CU; /**< @brief The offset of the primary output register to control the post dividers. */

        /**
         * @brief The control and status register.
         */
        struct Cs {
            static constexpr std::uintptr_t addr = pll_addr + cs_offset; /**< @brief The address of the cs register. */
            using cs_reg = reg_access::Reg<addr, reg_access::access_tag::mixed_access, std::uint32_t>; /**< @brief The cs register */

            /**
             * @brief The lock bit of the cs register.
             *
             * The lock bit indicates if the pll is locked and stable or not.
             */
            struct Lock {
                /**
                 * @brief The description of the lock bit.
                 */
                struct BitType {
                    using reg = cs_reg; /**< @brief The register to which the bits belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */
                    using bit_access = reg_access::access_tag::read; /**< @brief Access specifier of the specific bit. */

                    static constexpr T position = 31U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (0x01U << position); /**< @brief The mask of the bits. */

                    /**
                     * @brief Values to check if the pll is stable.
                     */
                    enum class value : T {
                        unstable = 0x0U, /**< @brief The output frequency is unstable. */
                        locked = 0x1U, /**< @brief The output frequency is stable. */
                    };
                };

                /**
                 * @brief The object to use the lock bits.
                 */
                using Bits = cs_reg::template Bits<BitType>;
                /**
                 * @brief The values of the lock bit.
                 */
                using Value = BitType::value;
            };

            /**
             * @brief The bypass bits of the cs register.
             *
             * If the bypass is enabled, the reference clock will be passed directly to the output instead of the divided
             * voltage controlled oscillator (VCO):
             */
            struct Bypass {
                static constexpr std::uint32_t bypass_pos = 8U; /**< @brief The position of the bypass bits in the cs register. */
                /**
                 * @brief The description of the bypass bits.
                 */
                using BitType = reg_access::BitTypeEnableDisable<cs_reg, bypass_pos, reg_access::access_tag::read_write>;
                /**
                 * @brief The object to use the bypass bit.
                 */
                using Bits = cs_reg::template Bits<BitType>;
                /**
                 * @brief The values of the bypass bit.
                 */
                using Value = BitType::value;
            };

            /**
             * @brief The reference divisor bits of the cs register.
             *
             * The input reference clock is divided by the value of the reference divisor bits.
             *
             * @tparam Divisor The divisor value.
             */
            template <std::uint32_t Divisor>
            struct RefDiv {
                /**
                 * @brief The description of the reference divisor bits.
                 */
                struct BitType {
                    using reg = cs_reg; /**< @brief The register to which the bits belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */
                    using bit_access = reg_access::access_tag::read_write; /**< @brief Access specifier of the specific bit. */

                    static constexpr T position = 0U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (0x3FU << position); /**< @brief The mask of the bits. */

                    static_assert((1 <= Divisor) && (Divisor <= 0x3FU), "Invalid ref divider.");

                    /**
                     * @brief The value to be set to the refdiv bits.
                     */
                    enum class value : T {
                        val = Divisor,
                    };
                };

                /**
                 * @brief The object to use the reference divisor bits.
                 */
                using Bits = cs_reg::template Bits<BitType>;
                /**
                 * @brief The value of the reference divisor bits.
                 */
                using Value = BitType::value;
            };
        };


        /**
         * @brief The pwr register controls the power to parts of the pll.
         */
        struct Pwr {
            static constexpr std::uintptr_t addr = pll_addr + pwr_offset; /**< @brief The address of the pwr register. */
            using pwr_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The pwr register */

            /**
             * @brief The VCO power down bits of the pwr register.
             *
             * Note: This bit enables/disables the power DOWN mode, not the power mode. Meaning if this bit is enabled,
             * the vco is shut down.
             */
            struct VcoPd {
                static constexpr std::uint32_t vco_pd_pos = 5U; /**< @brief The position of the VCO power down bits in the pwr register. */
                /**
                 * @brief The description of the vco power down bits.
                 */
                using BitType= reg_access::BitTypeEnableDisable<pwr_reg, vco_pd_pos>;
                /**
                 * @brief The object to use the vco power down bits.
                 */
                using Bits = pwr_reg::template Bits<BitType>;
                /**
                 * @brief The value of the vco power down bits.
                 */
                using Value = BitType::value;
            };

            /**
             * @brief The post divider power down bits of the pwr register.
             *
             * Note: This bit enables/disables the power DOWN mode, not the power mode. Meaning if this bit is enabled,
             * the post dividers are shut down.
             */
            struct PostDivPd {
                static constexpr std::uint32_t post_div_pd_pos = 3U; /**< @brief The position of the post divider power down bits in the pwr register. */
                /**
                 * @brief The description of the post divider power down bits.
                 */
                using BitType = reg_access::BitTypeEnableDisable<pwr_reg, post_div_pd_pos>;
                /**
                 * @brief The object to use the post divider power down bits.
                 */
                using Bits = pwr_reg::template Bits<BitType>;
                /**
                 * @brief The value of the post divider power down bits.
                 */
                using Value = BitType::value;
            };

            /**
             * @brief The pll power down bits of the pwr register.
             *
             * Note: This bit enables/disables the power DOWN mode, not the power mode. Meaning if this bit is enabled,
             * the pll is shut down.
             */
            struct Pd {
                static constexpr std::uint32_t pd_pos = 0U; /**< @brief The position of the power down bits in the pwr register. */
                /**
                 * @brief The description of the pll power down bits.
                 */
                using BitType = reg_access::BitTypeEnableDisable<pwr_reg, pd_pos>;
                /**
                 *
                 */
                using Bits = pwr_reg::template Bits<BitType>;
                using Value = BitType::value;
            };
        };

        /**
         * @brief The fbdiv_int register stores the feedback divider as an integer.
         */
        struct FbDivInt {
            static constexpr std::uintptr_t addr = pll_addr + fbdiv_int_offset; /**< @brief The address of the fbdiv_int register. */
            using fbdiv_int_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The fbdiv_int register. */


            /**
             * @brief The feedback divisor bits of the fbdiv_int register.
             *
             * @tparam Divisor The feedback divisor value.
             */
            template <std::uint32_t Divisor>
            struct FbDiv {
                /**
                 * @brief The description of the feedback divisor bits.
                 */
                struct BitType {
                    using reg = fbdiv_int_reg; /**< @brief The register to which the bits belongs. */
                    using T = reg::RegType; /**< @brief The type of the register. */
                    using bit_access = reg_access::access_tag::read_write; /**< @brief Access specifier of the specific bit. */

                    static constexpr T position = 0U; /**< @brief The position of the bits in the register. */
                    static constexpr T mask = (0xFFFU << position); /**< @brief The mask of the bits. */

                    static_assert((limits::fbdiv_min_val <= Divisor) && (Divisor <= limits::fbdiv_max_val), "Invalid feedback divider value. Allowed values 16 <= value <= 320");

                    /**
                     * @brief The value to which the fbdiv_int register is to be set.
                     */
                    enum class value : T {
                        val = Divisor,
                    };
                };

                /**
                 * @brief The object to use the feedback divisor bits.
                 */
                using Bits = fbdiv_int_reg::template Bits<BitType>;
                /**
                 * @brief The value of the feedback divisor bits.
                 */
                using Value = BitType::value;
            };

        };

        /**
         * @brief The prim register to set the post dividers.
         */
        struct Prim {
            static constexpr std::uintptr_t addr = pll_addr + prim_offset; /**< @brief The address of the prim register. */
            using prim_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The prim register. */


            static constexpr std::uint32_t postdiv1_pos = 16U; /**< @brief The position of the post divider 1 bits in the prim register. */
            static constexpr std::uint32_t postdiv2_pos = 12U; /**< @brief The position of the post divider 2 bits in the prim register. */
            /**
             * @brief The post divider bits.
             *
             * Post divider 1 and 2 are basically identically. The only difference is the position in the register.
             *
             * @tparam Divisor The value of the post divider.
             * @tparam Pos The position of the post divider bits in the register.
             */
            template <std::uint32_t Divisor, std::uint32_t Pos>
            struct PostDivBitTypeTemplate {
                using reg = prim_reg; /**< @brief The register to which the bits belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */
                using bit_access = reg_access::access_tag::read_write; /**< @brief Access specifier of the specific bit. */

                static_assert((Pos == postdiv1_pos) || (Pos == postdiv2_pos), "Invalid value for pos parameter!");

                static constexpr T position = Pos; /**< @brief The position of the bits in the register. */
                static constexpr T mask = (0x07U << position); /**< @brief The mask of the bits. */

                static_assert((limits::postdiv_min_val <= Divisor) && (Divisor <= limits::postdiv_max_val), "Invalid post divider value. Value must be between 1 and 7!");

                /**
                 * @brief The value to which the postdiv bits are to be set.
                 */
                enum class value : T {
                    val = Divisor,
                };
            };

            /**
             * @brief The post divider 1 bits are used to set the final output frequency of the pll.
             *
             * @tparam Divisor The post divider 1 value.
             */
            template <std::uint32_t Divisor>
            struct PostDiv1 {
                /**
                 * @brief The description of the post divider 1 bits.
                 */
                using BitType = PostDivBitTypeTemplate<Divisor, postdiv1_pos>;
                /**
                 * @brief The object to use the post divider 1 bits.
                 */
                using Bits = prim_reg::template Bits<BitType>;
                /**
                 * @brief The value of the post divider 1 bits.
                 */
                using Value = BitType::value;
            };

            /**
             * @brief The post divider 2 bits are used to set the final output frequency of the pll.
             *
             * @tparam Divisor The post divider 2 value.
             */
            template <std::uint32_t Divisor>
            struct PostDiv2 {
                /**
                 * @brief The description of the post divider 2 bits.
                 */
                using BitType = PostDivBitTypeTemplate<Divisor, postdiv2_pos>;
                /**
                 * @brief The object to use the post divider 2 bits.
                 */
                using Bits = prim_reg::template Bits<BitType>;
                /**
                 * @brief The value of the post divider 2 bits.
                 */
                using Value = BitType::value;
            };
        };
    };

    /** @}*/ // rp2040_pll
}

#endif // RP2040_SYSTEM_PLL_DEF_H
