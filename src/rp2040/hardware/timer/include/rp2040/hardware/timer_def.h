/**
 * @file timer.h
 * @brief The definitions of the Timer module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 15.10.2026
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
#ifndef RP204_HARDWARE_TIMER_DEF_H
#define RP204_HARDWARE_TIMER_DEF_H

#include <cstdint>

#include "rp2040/reg_access.h"

namespace rp2040::hardware::timer {
    /**
     * @addtogroup rp2040_timer
     * @{
     */

    /**
     * @brief The alarms of the timer module.
     */
    enum class Alarms : std::uint32_t {
        alarm0 = 0U,
        alarm1 = 1U,
        alarm2 = 2U,
        alarm3 = 3U,
    };

    /**
     * @brief The addresses of the timer module.
     */
    namespace address {
        static constexpr std::uintptr_t timer =  0x40054000U; /**< @brief Base address of the timer. */
    }


    /**
     * @brief Registers of the timer module.
     *
     * @tparam base Base address of the timer module.
     */
    template <std::uintptr_t base>
    struct TimerRegMap {
        static_assert(base == address::timer, "Invalid base address for TimerRegMap!");

        /**
         * @brief Offsets of the registers from the base address.
         */
        struct Offset {
            static constexpr std::uint32_t time_hw = 0x00U; /**< @brief Offset of the write high bits register. */
            static constexpr std::uint32_t time_lw = 0x04U; /**< @brief Offset of the write low bits register. */
            static constexpr std::uint32_t time_hr = 0x08U; /**< @brief Offset of the read high bits register. */
            static constexpr std::uint32_t time_lr = 0x0CU; /**< @brief Offset of the read low bits register. */
            static constexpr std::uint32_t alarm_base = 0x10U; /**< @brief Offset of the first alarm register. */
            static constexpr std::uint32_t armed = 0x20U; /**< @brief Offset of the armed register. */
            static constexpr std::uint32_t time_rawh = 0x24U; /**< @brief Offset of the raw read high bits register. */
            static constexpr std::uint32_t time_rawl = 0x28U; /**< @brief Offset of the raw read low bits register. */
            static constexpr std::uint32_t dbg_pause = 0x2CU; /**< @brief Offset of the debug pause register. */
            static constexpr std::uint32_t pause = 0x30U; /**< @brief Offset of the pause register. */
            static constexpr std::uint32_t intr = 0x34U; /**< @brief Offset of the raw interrupts register. */
            static constexpr std::uint32_t inte = 0x38U; /**< @brief Offset of the interrupt enable register. */
            static constexpr std::uint32_t intf = 0x3CU; /**< @biref Offset of the interrupt force register. */
            static constexpr std::uint32_t ints = 0x40U; /**< @brief Offset of the interrupt status register. */
        };

        /**
         * @brief Base register for the timer registers.
         *
         * The timer registers have the same structure, only varying in address and access specifier depending on their purpose.
         *
         * @tparam offset Offset of the specific timer register from the base address. @see Offset
         * @tparam AccessTag Register access tag of the specific timer register.
         */
        template <std::uint32_t offset, typename AccessTag>
        requires std::is_base_of_v<reg_access::access_tag::read, AccessTag> || std::is_base_of_v<reg_access::access_tag::write, AccessTag>
        struct TimerBase {
            static_assert(
                    offset == Offset::time_hw || offset == Offset::time_lw || offset == Offset::time_hr || offset == Offset::time_lr
                    || offset == Offset::time_rawh || offset == Offset::time_rawl, "Invalid offset for TimerBase!"
                );
            static constexpr std::uintptr_t addr = base + offset; /**< @brief Address of the timer register. */
            using timer_reg = reg_access::Reg<addr, AccessTag, std::uint32_t>; /**< @brief The timer register. */

            /**
             * @brief The timer bits.
             *
             * @tparam timer_value The value to which the timer register is to be set. Default value intended for timer read registers.
             */
            template <std::uint32_t timer_value = 0>
            struct Timer {
                /**
                 * @brief Description of the timer bits.
                 */
                struct BitType {
                    using reg = timer_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = timer_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0xFFFFFFFFU << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Value of the timer bits.
                     */
                    enum class value : T {
                        val = timer_value,
                    };
                };
                using Bits = timer_reg::template Bits<BitType>; /**< @brief Object to handle the timer bits. */
                using Value = BitType::value; /**< @brief Values of the timer bits. */
            };
        };

        /**
         * @brief Writes bits 63:32 of the timer. Always write @see TimeLW before TimeHW, as writes only get copied when
         * writing in this order.
         */
        using TimeHW = TimerBase<Offset::time_hw, reg_access::access_tag::write_fifo>;
        /**
         * @brief Writes bits 31:0 of the timer. Writes to this register must be followed with a write to @see TimeHW, as
         * writes only gets copied, when writing in this order.
         */
        using TimeLW = TimerBase<Offset::time_lw, reg_access::access_tag::write_fifo>;
        /**
         * @brief Reads bits 63:32 of the timer. Always read @see TimeLR before TimeHR. For non-latching reads @see TimeRawH
         */
        using TimeHR = TimerBase<Offset::time_hr, reg_access::access_tag::read>;
        /**
         * @brief Reads bits 31:0 of the timer. Reads to this register latches the timer, requiring a read from TimeHR to
         * clear the latch. For non-latching reads @see TimeRawL
         */
        using TimeLR = TimerBase<Offset::time_lr, reg_access::access_tag::read>;

        /**
         * @brief Arms an alarm and configures the time at which the alarm fires.
         *
         * @tparam alarm The alarm to be configured and armed.
         */
        template <Alarms alarm>
        struct AlarmBase {
            static constexpr std::uint32_t register_size_bytes {4U}; /**< @brief The register size of the alarm register. */
            static constexpr std::uintptr_t addr = base + Offset::alarm_base + std::to_underlying(alarm) * register_size_bytes; /**< @brief Address of the alarm register. */
            using alarm_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The alarm register. */

            /**
             * @brief The alarm bits to set the time when the alarm fires.
             *
             * The alarm fires when the value in this register equals the lower 32 bits of the tick counter. The alarm
             * will disarm itself after firing. The alarm can be disarmed before it fires via the @see Armed bits.
             *
             * @tparam alarm_time The time at which the alarm fires.
             */
            template <std::uint32_t alarm_time>
            struct Alarm {
                struct BitType {
                    using reg = alarm_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = alarm_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0xFFFFFFFFU << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Value of the timer bits.
                     */
                    enum class value : T {
                        val = alarm_time,
                    };
                };
                using Bits = alarm_reg::template Bits<BitType>; /**< @brief Object to handle the alarm bits. */
                using Value = BitType::value; /**< @brief Values of the alarm bits. */
            };
        };

        /**
         * @brief Indicates the status of each alarm (armed/disarmed). Alarms can be disarmed before firing by setting the
         * corresponding bit.
         */
        struct Armed {
            static constexpr std::uintptr_t addr = base + Offset::armed; /**< @brief Address of the armed register. */
            using armed_reg = reg_access::Reg<addr, reg_access::access_tag::write_clear, std::uint32_t>; /**< @brief The armed register. */

            /**
             * @brief The armed bit corresponding to the specific alarm.
             *
             * Setting this bit to high disables the alarm, while setting this bit to low has no effect. Reading high
             * means the alarm is enabled while reading low means the alarm is disabled.
             *
             * @tparam alarm The alarm to be checked/disabled.
             */
            template <Alarms alarm>
            struct Alarm {
                static constexpr std::uint32_t pos {std::to_underlying(alarm)}; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeHighLow<armed_reg, pos>; /**< @brief Description of the alarm bit. */
                using Bits = armed_reg::template Bits<BitType>; /**< @brief Object to handle the armed bits. */
                using Value = BitType::value; /**< @brief Values of the armed bits. */
            };
        };

        /**
         * @brief Non-latching register to read bits 63:32 of the timer.
         */
        using TimeRawH = TimerBase<Offset::time_rawh, reg_access::access_tag::read>;
        /**
         * @brief Non-latching register to read bits 31:0 of the timer.
         */
        using TimeRawL = TimerBase<Offset::time_rawl, reg_access::access_tag::read>;

        /**
         * @brief Enables/disables the pause of the timer when the corresponding debug port is active.
         */
        struct DbgPause {
            static constexpr std::uintptr_t addr = base + Offset::dbg_pause; /**< @brief Address of the debug pause register. */
            using dbg_pause_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The debug pause register. */

            /**
             * @brief Setting this bit high enabled the pause when processor 1 is in debug mode.
             */
            struct Dbg1 {
                static constexpr std::uint32_t pos = 2U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeHighLow<dbg_pause_reg, pos>; /**< @brief Description of the debug pause proc 1 bit.*/
                using Bits = dbg_pause_reg::template Bits<BitType>; /**< @brief Object to handle the debug pause proc 1 bit. */
                using Value = BitType::value; /**< @brief Value of the debug pause proc 1 bit. */
            };

            /**
             * @brief Setting this bit high enabled the pause when processor 0 is in debug mode.
             */
            struct Dbg0 {
                static constexpr std::uint32_t pos = 1U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeHighLow<dbg_pause_reg, pos>; /**< @brief Description of the debug pause proc 0 bit.*/
                using Bits = dbg_pause_reg::template Bits<BitType>; /**< @brief Object to handle the debug pause proc 0 bit. */
                using Value = BitType::value; /**< @brief Value of the debug pause proc 0 bit. */
            };
        };

        /**
         * @brief The pause register. Setting the pause bit high enables the pause.
         */
        struct Pause {
            static constexpr std::uintptr_t addr = base + Offset::pause; /**< @brief Address of the pause register. */
            using pause_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The pause register. */

            static constexpr std::uint32_t pos = 0U; /**< @brief Position of the bit in the register. */
            using BitType = reg_access::BitTypeHighLow<pause_reg, pos>; /**< @brief Description of the pause bit.*/
            using Bits = pause_reg::template Bits<BitType>; /**< @brief Object to handle the pause bit. */
            using Value = BitType::value; /**< @brief Values of the pause bit. */
        };

        /**
         * @brief Base register type to handle/configure/check the interrupt of the timer alarms.
         *
         * @tparam offset Offset of the interrupt register from the timer base address.
         */
        template <std::uint32_t offset>
        struct InterruptBase {
            static_assert(offset == Offset::intr || offset == Offset::inte || offset == Offset::intf || offset == Offset::ints, "Invalid offset for InterruptBase!");
            static constexpr std::uintptr_t addr = base + offset; /**< @brief Address of the interrupt register. */
            using AccessTag = decltype([]() {
                if constexpr (offset == Offset::intr) {
                    return std::type_identity_t<reg_access::access_tag::write_clear>();
                } else if constexpr ((offset == Offset::inte) || (offset == Offset::intf)) {
                    return std::type_identity_t<reg_access::access_tag::read_write>();
                } else {
                    return std::type_identity_t<reg_access::access_tag::read>();
                }
            } ());
            using interrupt_reg = reg_access::Reg<addr, AccessTag, std::uint32_t>; /**< @brief The interrupt register. */

            /**
             * @brief The alarm bit corresponding to the specified alarm.+
             *
             * @tparam alarm The alarm for which the interrupt is to configured/status to be read.
             */
            template <Alarms alarm>
            struct Alarm {
                static constexpr std::uint32_t pos {std::to_underlying(alarm)}; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeHighLow<interrupt_reg, pos>; /**< @brief Description of the alarm bit. */
                using Bits = interrupt_reg::template Bits<BitType>; /**< @brief Object to handle the alarm bit. */
                using Value = BitType::value; /**< @brief Values of the alarm bit. */
            };
        };

        using IntR = InterruptBase<Offset::intr>; /**< @brief Raw interrupts register. */
        using IntE = InterruptBase<Offset::inte>; /**< @brief Interrupt enable register. */
        using IntF = InterruptBase<Offset::intf>; /**< @brief Interrupt force register. */
        using IntS = InterruptBase<Offset::ints>; /**< @brief Interrupt status after masking & forcing register. */
    };

    /**@]*/
}

#endif // RP204_HARDWARE_TIMER_DEF_H
