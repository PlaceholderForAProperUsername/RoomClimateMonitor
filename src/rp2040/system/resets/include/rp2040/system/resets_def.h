//
//  on 16.06.26.
//

#ifndef RP2040_SYSTEM_RESETS_DEF_H
#define RP2040_SYSTEM_RESETS_DEF_H

#include <cstdint>

#include "utils/reg_access.h"

namespace rp2040::system {
    /**
     * @addtogroup rp2040_resets
     * @{
     */

    constexpr std::uintptr_t resets_base = 0x4000c000; /**< @brief The base address of the reset controller. */

    /**
     * @brief The subsystems controllable by the reset controller.
     */
    enum class SubsystemBits: std::uint32_t {
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

    template <std::uintptr_t resets_addr>
    struct Resets_RegMapType {
        static_assert(resets_addr == resets_base, "The resets address must be a valid address");

        static constexpr std::uint32_t reset_offset = 0x00U; /**< @brief The offset for the reset register. */
        static constexpr std::uint32_t wdsel_offset = 0x04U; /**< @brief The offset for the watchdog select (wdsel) register. */
        static constexpr std::uint32_t reset_done_offset = 0x08U; /**< @brief The offset for the reset done register. */

        /**
         * @brief The reset register of the reset controller.
         *
         * If a bit is set, the subsystem is held in reset. To use the subsystem, the reset must be deasserted by setting
         * the bit to 0.
         */
        struct reset {
            static constexpr std::uint32_t addr = resets_base + reset_offset; /**< @brief Address of the reset register. */
            using reset_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The reset register type */


            /**
             * @brief The BitField representing the bits of the reset register.
             *
             * @tparam subsystem The subsystem to be controlled.
             */
            template <SubsystemBits subsystem>
            struct ResetsBitField {
                using reg = reset_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static_assert(static_cast<T>(subsystem) < static_cast<T>(SubsystemBits::NumberOfSubsystems), "Invalid subsystem!");

                static constexpr T position = static_cast<T>(subsystem); /**< @brief The position of the bits in the register */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the affected bits. */

                /**
                 * @brief The valid values for the bits.
                 */
                enum class value : T {
                    enable = 0U, /**< @brief Deassert the reset and thus enable the subsystem. */
                    disable = 1U, /**< @brief Put the subsystem in reset and thus disable it. */
                };
            };

            /**
             * @brief The ResetsBitField type.
             *
             * @tparam subsystem The subsystem to be controlled.
             */
            template <SubsystemBits subsystem>
            using reset_bits = reset_reg::Bits<ResetsBitField<subsystem>>;
        };

        /**
         * @brief The watchdog select register.
         *
         * If a bit is set and the watchdog fires, the subsystem will reset.
         */
        struct wdsel {
            static constexpr std::uint32_t addr = resets_base + wdsel_offset; /**< @brief The address of the wdsel register. */
            using wdsel_reg = utils::reg_access::Reg<addr, utils::reg_access::read_write_access, std::uint32_t>; /**< @brief The wdsel register. */

            /**
             * @brief The bits of the watchdog select register.
             *
             * @tparam subsystem The subsystem to be selected.
             */
            template <SubsystemBits subsystem>
            struct WDSelBitField {
                using reg = wdsel_reg; /**< @brief The register to which the bitfield belongs. */
                using T = reg::RegType; /**< @brief The type of the register. */

                static_assert(static_cast<T>(subsystem) < static_cast<T>(SubsystemBits::NumberOfSubsystems), "Invalid subsystem!");

                static constexpr T position = static_cast<T>(subsystem); /**< @brief The position of the bits in the register */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the affected bits. */

                /**
                 * @brief The valid values for the bits.
                 */
                enum class value : T {
                    disable = 0U, /**< @brief The subsystem will be unaffected when the watchdog fires */
                    enable = 1U, /**< @brief The subsystem will be put in reset when the watchdog fires */
                };
            };

            /**
             * @brief The watchdog select BitField type.
             *
             * @tparam subsystem The subsystem to be selected.
             */
            template <SubsystemBits subsystem>
            using wdsel_bits = wdsel_reg::Bits<WDSelBitField<subsystem>>;
        };

        /**
         * @brief The reset_done register.
         *
         * If a bit is set for a subsystem, the reset for that subsystem is deasserted, meaning the registers are ready to
         * be accessed.
         */
        struct reset_done {
            static constexpr std::uint32_t addr = resets_base + reset_done_offset; /**< @brief The address of the reset_done register. */
            using reset_done_reg = utils::reg_access::Reg<addr, utils::reg_access::read_access, std::uint32_t>; /**< @brief The reset_done register. */

            /**
             * @brief The bits of the reset_done register.
             *
             * @tparam subsystem The subsystem whose status is to be checked.
             */
            template <SubsystemBits subsystem>
            struct ResetDoneBitField {
                using reg = reset_done_reg;
                using T = reg::RegType;

                static_assert(static_cast<T>(subsystem) < static_cast<T>(SubsystemBits::NumberOfSubsystems), "Invalid subsystem!");

                static constexpr T position = static_cast<T>(subsystem); /**< @brief The position of the bits in the register */
                static constexpr T mask = (0x01U << position); /**< @brief The mask of the affected bits. */

                /**
                 * @brief The meaning of the bits.
                 */
                enum class value : T {
                    disabled = 0U, /**< @brief The subsystem is still in reset and thus disabled. */
                    enabled = 1U /**< @brief The subsystem is out of reset and thus enabled. */
                };
            };

            /**
             * @brief The reset_done Bits type.
             *
             * @tparam subsystem The subsystem whose status is to be checked.
             */
            template <SubsystemBits subsystem>
            using reset_done_bits = reset_done_reg::Bits<ResetDoneBitField<subsystem>>;
        };
    };

    /** @}*/ // rp2040_reset
}


#endif // RP2040_SYSTEM_RESETS_DEF_H
