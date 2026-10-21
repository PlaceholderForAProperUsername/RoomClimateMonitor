/**
 * @file watchdog_def.h
 * @brief The definitions of the watchdog module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 20.10.2026
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

#ifndef RP2040_HARDWARE_WATCHDOG_DEF_H
#define RP2040_HARDWARE_WATCHDOG_DEF_H

#include <cstdint>

#include "rp2040/reg_access.h"

namespace rp2040::hardware::watchdog {
    /**
     * @addtogroup rp2040_watchdog
     * @{
     */

    /**
     * @brief Addresses of the watchdog module.
     */
    namespace address {
        constexpr std::uintptr_t watchdog {0x40058000U}; /**< @brief Base address of the watchdog module. */
    }

    template <std::uintptr_t base>
    struct RegMap {
        static_assert(base == address::watchdog, "Invalid base address for watchdog RegMap!");

        /**
         * @brief Register offsets of the watchdog module.
         */
        struct Offset {
            static constexpr std::uint32_t ctrl {0x00U}; /**< @brief Offset of the control register. */
            static constexpr std::uint32_t load {0x04U}; /**< @brief Offset of the load register. */
            static constexpr std::uint32_t reason {0x08U}; /**< @brief Offset of the reason register. */
            static constexpr std::uint32_t scratch0 {0x0CU}; /**< @brief Offset of the scratch0 register. */
            static constexpr std::uint32_t scratch1 {0x10U}; /**< @brief Offset of the scratch1 register. */
            static constexpr std::uint32_t scratch2 {0x14U}; /**< @brief Offset of the scratch2 register. */
            static constexpr std::uint32_t scratch3 {0x18U}; /**< @brief Offset of the scratch3 register. */
            static constexpr std::uint32_t scratch4 {0x1CU}; /**< @brief Offset of the scratch4 register. */
            static constexpr std::uint32_t scratch5 {0x20U}; /**< @brief Offset of the scratch5 register. */
            static constexpr std::uint32_t scratch6 {0x24U}; /**< @brief Offset of the scratch6 register. */
            static constexpr std::uint32_t scratch7 {0x28U}; /**< @brief Offset of the scratch7 register. */
            static constexpr std::uint32_t tick {0x2CU}; /**< @brief Offset of the tick register. */
        };

        /**
         * @brief Watchdog control register.
         */
        struct Ctrl {
            static constexpr std::uintptr_t addr {base + Offset::ctrl}; /**< @brief Address of the watchdog register. */
            using ctrl_reg = reg_access::Reg<addr, reg_access::access_tag::mixed_access, std::uint32_t>; /**< @brief The watchdog register. */

            /**
             * @brief Triggers a watchdog reset.
             */
            struct Trigger {
                static constexpr std::uint32_t pos {31U}; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<ctrl_reg, pos, reg_access::access_tag::self_clear>; /**< @brief Description of the trigger bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the trigger bits. */
                using Value = BitType::value; /**< @brief Values of the trigger bits. */
            };

            /**
             * @brief Enables the watchdog timer.
             */
            struct Enable {
                static constexpr std::uint32_t pos {30U}; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<ctrl_reg, pos, reg_access::access_tag::read_write>; /**< @brief Description of the enable bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the enable bits. */
                using Value = BitType::value; /**< @brief Values of the enable bits. */
            };

            /**
             * @brief Pause debug 1 bit enables the pause mode when processor 1 is in debug mode.
             */
            struct PauseDbg1 {
                static constexpr std::uint32_t pos {26U}; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeHighLow<ctrl_reg, pos, reg_access::access_tag::read_write>; /**< @brief Description of the pause debug 1 bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the pause debug 1 bits. */
                using Value = BitType::value; /**< @brief Values of the pause debug 1 bits. */
            };

            /**
             * @brief Pause debug 0 bit enables the pause mode when processor 0 is in debug mode.
             */
            struct PauseDbg0 {
                static constexpr std::uint32_t pos {25U}; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeHighLow<ctrl_reg, pos, reg_access::access_tag::read_write>; /**< @brief Description of the pause debug 0 bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the pause debug 0 bits. */
                using Value = BitType::value; /**< @brief Values of the pause debug 0 bits. */
            };

            /**
             * @brief Pause JTAG bit enables the pause mode when JTAG is accessing the bus fabric.
             */
            struct PauseJtag {
                static constexpr std::uint32_t pos {25U}; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeHighLow<ctrl_reg, pos, reg_access::access_tag::read_write>; /**< @brief Description of the pause JTAG bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the pause JTAG bits. */
                using Value = BitType::value; /**< @brief Values of the pause JTAG bits. */
            };

            /**
             * @brief Indicates the number of ticks before a watchdog reset will be triggered.
             *
             * Due to logic error, the counter is decremented twice per tick, meaning the value of the time bits must be
             * divided by 2 to get the actual remaining time.
             */
            struct Time {
                /**
                 * @brief Description of the time bits.
                 */
                struct BitType {
                    using reg = ctrl_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = reg_access::access_tag::read; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0xFFFFFFU << position); /**< @brief The mask of the bit used in bit operations. */
                };
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to handle the time bits. */
            };
        };
    };

    /**@}*/ //rp2040_watchdog
}

#endif // RP2040_HARDWARE_WATCHDOG_DEF_H
