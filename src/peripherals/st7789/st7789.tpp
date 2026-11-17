/**
 * @file st7789.tpp
 * @brief Firmware to use a display with a st7789 driver.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 12.11.2026
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


#ifndef ST7789_TPP
#define ST7789_TPP

#include "st7789.h"

namespace st7789 {
    /**
     * @addtogroup st7789
     * @{
     */

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType>
    requires GpioTypeConcept<GpioType, GpioPinType>
    void St7789<SpiType, TimerType, GpioType, GpioPinType>::init() {
        constexpr std::array<std::uint8_t, 1> sw_reset_cmd {commands::sw_reset.first};
        this->m_spi.write_blocking(sw_reset_cmd);
        constexpr std::uint32_t wait_after_reset_ms {120};
        this->m_timer.busy_wait_ms_32(wait_after_reset_ms);

        this->m_flags.reset();
        this->m_flags.set(std::to_underlying(Flags::initialized));
    }

    /**@}*/
}

#endif //ST7789_TPP