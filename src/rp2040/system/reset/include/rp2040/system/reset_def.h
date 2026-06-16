//
//  on 16.06.26.
//

#ifndef RP2040_SYSTEM_RESET_DEF_H
#define RP2040_SYSTEM_RESET_DEF_H

#include <cstdint>

namespace rp2040::system {
    /**
     * @addtogroup rp2040_reset
     * @{
     */

    constexpr std::uintptr_t resets_base = 0x4000c000; /**< @brief The base address of the reset controller. */

    /**
     * @brief The subsystems controllable by the reset controller.
     */
    enum class ResetBits: std::uint32_t {
        ADC = 0U,
        BUSCTRL = 1U,
        DMA = 2U,
        I2C0 = 3U,
        I2C1 = 4U,
        IO_BANK0 = 5U,
        IO_QSPI = 6U,
        JTAG = 7U,
        PADS_BANK0 = 8U,
        PADS_QSPI = 9U,
        PIO0 = 10U,
        PIO1 = 11U,
        PLL_SYS = 12U,
        PLL_USB = 13U,
        PWM = 14U,
        RTC = 15U,
        SPI0 = 16U,
        SPI1 = 17U,
        SYSCFG = 18U,
        SYSINFO = 19U,
        TBMAN = 20U,
        TIMER = 21U,
        UART0 = 22U,
        UART1 = 23U,
        USBCTRL = 24U,
        NumberOfSubsystems
    };


    /** @}*/ // rp2040_reset

}


#endif // RP2040_SYSTEM_RESET_DEF_H
