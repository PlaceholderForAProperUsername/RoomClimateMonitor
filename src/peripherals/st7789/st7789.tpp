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

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::init() {
        // data_cmd and chip_select are active low
        if (this->m_gpio.template set_output<s_data_cmd, true>() != status::Status::OK) {
            return status::Status::ERROR_CONFIG;
        }
        if (this->m_gpio.template set_output<s_chip_select, true>() != status::Status::OK) {
            return status::Status::ERROR_CONFIG;
        }
        this->send_cmd(commands::sw_reset);
        constexpr std::uint32_t wait_after_reset_ms {120};
        this->m_timer.busy_wait_ms_32(wait_after_reset_ms);

        this->m_flags.reset();
        this->m_flags.set(std::to_underlying(Flags::initialized));

        return status::Status::OK;
    }

    template <SpiTypeConcept SpiType, TimerTypeConcept TimerType, typename GpioType, typename GpioPinType, GpioPinType data_cmd, GpioPinType chip_select>
    requires GpioTypeConcept<GpioType, GpioPinType>
    status::Status St7789<SpiType, TimerType, GpioType, GpioPinType, data_cmd, chip_select>::send_cmd(std::pair<std::array<std::uint8_t, 1>, std::size_t> cmd, std::span<std::uint8_t> data) {
        // write memory command has a variable amount of data that can be sent, therefore it is checked extra.
        // the other commands have fixed size of parameters to be sent.
        if (((cmd == commands::write_memory) && data.empty()) || ((cmd != commands::write_memory) && cmd.second != data.size()))  {
            return status::Status::ERROR_PARAM;
        }
        this->m_gpio.template set_output<s_data_cmd, false>();
        this->m_gpio.template set_output<s_chip_select, false>();
        this->m_spi.write_blocking(cmd.first);
        this->m_gpio.template set_output<s_data_cmd, true>();
        if (cmd.second > 0) {
            this->m_spi.write_blocking(data);
        }
        this->m_gpio.template set_output<s_chip_select, true>();

        return status::Status::OK;
    }

    /**@}*/
}

#endif //ST7789_TPP