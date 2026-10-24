/**
 * @file nvic_def.h
 * @brief The definitions of the NVIC module of the rp2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 24.10.2026
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

#ifndef RP2040_SYSTEM_NVIC_DEF_H
#define RP2040_SYSTEM_NVIC_DEF_H

#include <cstdint>
#include <utility>

#include "rp2040/reg_access.h"

namespace rp2040::system::nvic {
    /**
     * @addtogroup rp2040_nvic
     * @{
     */

    /**
     * @brief Available interrupts of the RP2040.
     */
    enum class Interrupts : std::uint32_t {
        timer_irq_0 = 0U,
        timer_irq_1 = 1U,
        timer_irq_2 = 2U,
        timer_irq_3 = 3U,
        pwm_irq_wrap = 4U,
        usbctrl_irq = 5U,
        xip_irq = 6U,
        pio0_irq_0 = 7U,
        pio0_irq_1 = 8U,
        pio1_irq_0 = 9U,
        pio1_irq_1 = 10U,
        dma_irq_0 = 11U,
        dma_irq_1 = 12U,
        io_irq_bank0 = 13U,
        io_irq_qspi = 14U,
        sio_irq_proc0 = 15U,
        sio_irq_proc1 = 16U,
        clocks_irq = 17U,
        spio0_irq = 18U,
        spio1_irq = 19U,
        uart0_irq = 20U,
        uart1_irq = 21U,
        adc_irq_fifo = 22U,
        i2c0_irq = 23U,
        i2c1_irq = 24U,
        rtc_irq = 25U,
        number_of_interrupts
    };

    /**
     * @brief Base addresses of the NVIC module.
     */
    namespace address {
        constexpr std::uintptr_t nvic { 0xE000E100U}; /**< @brief Base address of the NVIC. */
    }

    /**
     * @brief The register map of the NVIC module.
     *
     * @tparam base The base address of the NVIC.
     */
    template <std::uintptr_t base>
    struct RegMap {
        static_assert(base == address::nvic, "Invalid base address for NVIC register map!");

        /**
         * @brief Offsets from the NVIC base address.
         */
        struct Offset {
            static constexpr std::uint32_t iser {0x000U}; /**< @brief Offset of the interrupt set-enable register. */
            static constexpr std::uint32_t icer {0x080U}; /**< @brief Offset of the interrupt clear-enable register. */
            static constexpr std::uint32_t ispr {0x100U}; /**< @brief Offset of the interrupt set-pending register. */
            static constexpr std::uint32_t icpr {0x180U}; /**< @brief Offset of the interrupt clear-pending register. */
            static constexpr std::uint32_t ipr_base {0x300U}; /**< @brief Offset of the base interrupt priority registers. */
        };

        /**
         * @brief Interrupt set-enable register.
         *
         * If an interrupt is pending and then gets enabled, the NVIC activates the interrupt based on its priority.
         */
        struct Iser {
            static constexpr std::uintptr_t addr {base + Offset::iser}; /**< @brief Address of the interrupt set-enable register. */
            using iser_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The interrupt set-enable register. */

            /**
             * @brief Set enable bits.
             *
             * The interrupt can only be enabled with a write. To disable an interrupt @see Icer
             *
             * @tparam interrupt The interrupt to be enabled.
             */
            template <Interrupts interrupt>
            struct SetEnable {
                static constexpr auto pos {std::to_underlying(interrupt)}; /**< @brief Position of the interrupt in the register. */
                using BitType = reg_access::BitTypeEnableDisable<iser_reg, pos>; /**< @brief Description of the interrupt set enable bit. */
                using Bits = iser_reg::template Bits<BitType>; /**< @brief Object to handle the interrupt set enable bit. */
                using Value = BitType::value; /**< @brief Value of the interrupt set enable bit. */
            };
        };

        /**
         * @brief Interrupt clear-enable register.
         */
        struct Icer {
            static constexpr std::uintptr_t addr {base + Offset::icer}; /**< @brief Address of the interrupt clear-enable register. */
            using iser_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The interrupt clear-enable register. */

            /**
             * @brief Set enable bits.
             *
             * The interrupt can only be disabled with a write. To enable an interrupt @see Iser
             * Note: writing a 1 disables an interrupt but reading a 1 means the interrupt is enabled. The interrupt is disabled
             * when a read returns 0 for the corresponding bit.
             *
             * @tparam interrupt The interrupt to be disabled.
             */
            template <Interrupts interrupt>
            struct ClearEnable {
                static constexpr auto pos {std::to_underlying(interrupt)}; /**< @brief Position of the interrupt in the register. */
                using BitType = reg_access::BitTypeHighLow<iser_reg, pos>; /**< @brief Description of the interrupt clear enable bit. */
                using Bits = iser_reg::template Bits<BitType>; /**< @brief Object to handle the interrupt clear enable bit. */
                using Value = BitType::value; /**< @brief Value of the interrupt clear enable bit. */
            };
        };
    };

    /**@}*/
}

#endif // RP2040_SYSTEM_NVIC_DEF_H
