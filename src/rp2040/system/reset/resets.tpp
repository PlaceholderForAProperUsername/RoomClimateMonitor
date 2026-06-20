/**
 *  @file resets.tpp
 * @brief Implementation of the resets controller.
 * @author Thorsten Hoffmeister
 * @version 0.1
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


#ifndef RP2040_SYSTEM_RESETS_T_H
#define RP2040_SYSTEM_RESETS_T_H

#include "resets.h"
#include "include/rp2040/system/resets.h"

namespace rp2040::system {
    /**
     * @addtogroup rp2040_resets
     * @{
     */

    template <std::uintptr_t resets_addr>
    Resets_Type<resets_addr>& Resets_Type<resets_addr>::getInstance() {
        static Resets_Type<resets_addr> instance;
        return instance;
    }


    /** @} */
} // rp2040::system

#endif // RP2040_SYSTEM_RESETS_T_H