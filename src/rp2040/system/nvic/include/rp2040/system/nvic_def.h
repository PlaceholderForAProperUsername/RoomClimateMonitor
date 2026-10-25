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
            using icer_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The interrupt clear-enable register. */

            /**
             * @brief Clear enable bits.
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
                using BitType = reg_access::BitTypeHighLow<icer_reg, pos>; /**< @brief Description of the interrupt clear enable bit. */
                using Bits = icer_reg::template Bits<BitType>; /**< @brief Object to handle the interrupt clear enable bit. */
                using Value = BitType::value; /**< @brief Value of the interrupt clear enable bit. */
            };
        };

        /**
         * @brief Interrupt set-pending register.
         */
        struct Ispr {
            static constexpr std::uintptr_t addr {base + Offset::ispr}; /**< @brief Address of the interrupt set-pending register. */
            using ispr_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The interrupt set-pending register. */

            /**
             * @brief Set pending bits.
             *
             * Writing a 1 set the interrupt to be pending while writing a 0 has no effect. Reading a 1 means the interrupt
             * is already pending and reading 0 means the interrupt is not pending.
             *
             * @tparam interrupt The interrupt to be set to be pending.
             */
            template <Interrupts interrupt>
            struct SetPending {
                static constexpr auto pos {std::to_underlying(interrupt)}; /**< @brief Position of the interrupt in the register. */
                using BitType = reg_access::BitTypeHighLow<ispr_reg, pos>; /**< @brief Description of the interrupt set pending bit. */
                using Bits = ispr_reg::template Bits<BitType>; /**< @brief Object to handle the interrupt set pending bit. */
                using Value = BitType::value; /**< @brief Value of the interrupt set pending bit. */
            };
        };

        /**
         * @brief Interrupt clear-pending register.
         */
        struct Icpr {
            static constexpr std::uintptr_t addr {base + Offset::ispr}; /**< @brief Address of the interrupt clear-pending register. */
            using icpr_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The interrupt clear-pending register. */

            /**
             * @brief Set pending bits.
             *
             * Writing a 1 removes the interrupt pending state while writing a 0 has no effect. Reading a 1 means the interrupt
             * is already pending and reading 0 means the interrupt is not pending.
             *
             * @tparam interrupt The interrupt to be cleared from pending.
             */
            template <Interrupts interrupt>
            struct SetPending {
                static constexpr auto pos {std::to_underlying(interrupt)}; /**< @brief Position of the interrupt in the register. */
                using BitType = reg_access::BitTypeHighLow<icpr_reg, pos>; /**< @brief Description of the interrupt clear pending bit. */
                using Bits = icpr_reg::template Bits<BitType>; /**< @brief Object to handle the interrupt clear pending bit. */
                using Value = BitType::value; /**< @brief Value of the interrupt clear pending bit. */
            };
        };

        /**
         * @brief The interrupt priority register.
         *
         * Sets the priority level for interrupts with 0 being the highest priority and 3 the lowest.
         *
         * There are 8 interrupt priority registers in total, each containing 4 interrupts starting from interrupt 0 in
         * interrupt priority register 0 going to interrupt 31 in interrupt priority register 7. A byte is reserved for
         * each interrupt and the priority bits are located in the higher two bits.
         *
         * @tparam interrupt The interrupt for which the priority is to be set.
         */
        template <Interrupts interrupt>
        struct IprBase {
            static constexpr std::uint32_t interrupts_per_register {4U}; /**< @brief The number of interrupts within a register. */
            static constexpr std::uint32_t register_size_bytes {4U}; /**< @brief The size of the register in bytes. */
            static constexpr std::uint32_t reg_offset {register_size_bytes * (std::to_underlying(interrupt) / interrupts_per_register)}; /**< @brief The interrupt priority register offset from the first interrupt priority register. */
            static constexpr std::uintptr_t addr {base + Offset::ipr_base + reg_offset}; /**< @brief The address of the interrupt priority register of the interrupt for which the priority is to be set. */
            using ipr_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The interrupt priority register of the interrupt for which the priority is to be set. */

            /**
             * @brief The priority bits of the interrupt.
             *
             * @tparam priority The priority of the interrupt with 0 being highest priority and 3 the lowest priority.
             */
            template <std::uint32_t priority>
            struct Priority {
                static constexpr std::uint32_t bits_reserved_per_interrupt {8U}; /**< @brief A byte is reserved for each interrupt. */
                static constexpr std::uint32_t interrupt_bits_offset {6U}; /**< @brief The priority bits are located in the two highest bits of the reserved byte. */
                static constexpr std::uint32_t pos {(bits_reserved_per_interrupt * (std::to_underlying(interrupt) % interrupts_per_register)) + interrupt_bits_offset}; /**< @brief The bit position of the priority bits. */

                static constexpr std::uint32_t mask {0x03U}; /**< @brief The mask of the priority bits. */

                using BitType = reg_access::BitTypeValues<priority, ipr_reg, mask, pos>; /**< @brief Description of the priority bits. */
                using Bits = ipr_reg::template Bits<BitType>; /**< @brief Object to handle the priority bits. */
                using Value = BitType::value; /**< @brief Value of the priority bits. */
            };
        };
    };

    /**@}*/
}

#endif // RP2040_SYSTEM_NVIC_DEF_H
