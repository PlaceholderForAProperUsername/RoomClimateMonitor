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
        NvicType& get_instance();

    private:
        NvicType() = default;
    };

    using NVIC = NvicType<address::nvic>; /**< @brief The NVIC controller. */
    /**@}*/
}

#include "../../../nvic.tpp"

#endif // RP2040_SYSTEM_NVIC_H
