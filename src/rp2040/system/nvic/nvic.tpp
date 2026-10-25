/**
 * @file nvic.tpp
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


#ifndef RP2040_SYSTEM_NVIC_TPP
#define RP2040_SYSTEM_NVIC_TPP

#include "rp2040/system/nvic.h"

namespace rp2040::system::nvic {
    /**
     * @addtogroup rp2040_nvic
     * @{
     */

    template <std::uintptr_t base>
    NvicType<base>& NvicType<base>::get_instance() {
        static NvicType<base> instance;
        return instance;
    }

    template <std::uintptr_t base>
    template <Interrupts interrupt>
    void NvicType<base>::enable_interrupt() {
        using EnableBits = Registers::Iser::template SetEnable<interrupt>;
        EnableBits::Bits::set(EnableBits::Value::enable);
    }

    /**@}*/
}

#endif // RP2040_SYSTEM_NVIC_TPP