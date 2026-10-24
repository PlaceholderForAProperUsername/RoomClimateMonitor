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

namespace rp2040::system::nvic {
    /**
     * @addtogroup rp2040_nvic
     * @{
     */

    /**
     * @brief Available interrupts of the RP2040.
     */
    enum class Interrupts : std::uint8_t {
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

    /**@}*/
}

#endif // RP2040_SYSTEM_NVIC_DEF_H
