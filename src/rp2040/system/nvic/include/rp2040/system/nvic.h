/**
*  @file nvic.h
 * @brief The NVIC module of the rp2040.
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

#ifndef RP2040_SYSTEM_NVIC_H
#define RP2040_SYSTEM_NVIC_H

#include "nvic_def.h"

namespace rp2040::system::nvic {
    /**
     * @addtogroup rp2040_nvic
     * @{
     */

    /**
     * @brief Nested vectored interrupt controller (NVIC) controller.
     *
     * @tparam base Base address of the NVIC controller.
     */
    template <std::uintptr_t base>
    class NvicType {
        static_assert(base == address::nvic, "Invalid base address for NVIC!");

    public:
        /**
         * @brief Getter function to get the Singleton instance.
         *
         * @return NvicType& The Singleton instance.
         */
        static NvicType& get_instance();

        /**
         * @brief Enables a specific interrupt.
         *
         * A pending interrupt is activated based on its priority. To prevent this, clear the pending status before
         * enabling the interrupt. @see clear_pending()
         *
         * @tparam interrupt The interrupt to be enabled.
         */
        template <Interrupts interrupt>
        void enable_interrupt();

        /**
         * @brief Disables a specific interrupt.
         *
         * @tparam interrupt The interrupt to be disabled.
         */
        template <Interrupts interrupt>
        void disable_interrupt();

        /**
         * @brief Sets the status of an interrupt to pending.
         *
         * @tparam interrupt The interrupt which status is to be set to pending.
         */
        template <Interrupts interrupt>
        void set_pending();

        /**
         * @brief Removes pending status from an interrupt.
         *
         * @tparam interrupt The interrupt for which the pending status is to be cleared.
         */
        template <Interrupts interrupt>
        void clear_pending();

        /**
         * @brief Sets the priority of an interrupt.
         *
         * Highest priority: 0
         * Lowest priority: 3
         *
         * @tparam interrupt The interrupt for which the priority is to be set.
         * @tparam priority The priority of the interrupt.
         */
        template <Interrupts interrupt, std::uint8_t priority>
        void set_priority();

    private:
        using Registers = RegMap<base>;

        NvicType() = default;
    };

    using NVIC = NvicType<address::nvic>; /**< @brief The NVIC controller. */
    /**@}*/
}

#include "../../../nvic.tpp"

#endif // RP2040_SYSTEM_NVIC_H
