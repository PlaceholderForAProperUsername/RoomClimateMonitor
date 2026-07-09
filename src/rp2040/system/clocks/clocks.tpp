/**
 * @file clocks.tpp
 * @brief The Clocks module to use the clocks of the RP2040.
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

#ifndef RP2040_SYSTEM_CLOCKS_T_H
#define RP2040_SYSTEM_CLOCKS_T_H
#include "rp2040/system/clocks.h"

namespace rp2040::system::clocks {

    template <typename ClockDefType>
    requires IsClockType<ClockDefType>
    ClockBaseType<ClockDefType>& ClockBaseType<ClockDefType>::getInstance() {
        static ClockBaseType<ClockDefType> instance;
        return instance;
    }


}

#endif // RP2040_SYSTEM_CLOCKS_T_H