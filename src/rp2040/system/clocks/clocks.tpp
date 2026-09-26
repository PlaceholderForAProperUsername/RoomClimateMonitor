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

#ifndef RP2040_SYSTEM_CLOCKS_T
#define RP2040_SYSTEM_CLOCKS_T
#include "rp2040/system/clocks.h"
#include "rp2040/system/clocks_def.h"

namespace rp2040::system::clocks {

    template <ClockLayoutConcept ClockLayout>
    ClockBaseType<ClockLayout>& ClockBaseType<ClockLayout>::get_instance() {
        static ClockBaseType<ClockLayout> instance;
        return instance;
    }

    template <ClockLayoutConcept ClockLayout>
    template <ClockConfType<ClockLayout> conf>
    std::enable_if_t<(ClockLayout::Ctrl::src_max_val > 0), void> ClockBaseType<ClockLayout>::configure() {

        // If the divisor is increased, the divisor should be set before the source to reduce momentary overspeed.
        // This is according to the RP2040's datasheet.
        if constexpr (ClockLayout::Div::int_mask > 0) {
            constexpr std::uint32_t div_val = []() -> std::uint32_t {
                if constexpr (ClockLayout::Div::frac_mask > 0) {
                    return ((conf.div_int & 0xFFFFFFU) << 8) | conf.div_frac;
                } else {
                    return (conf.div_int & 0xFFFFFFU) << 8;
                }
            }();
            if (div_r::div_reg::read() > (div_val)) {
                if constexpr ((ClockLayout::Div::int_mask > 0) && (ClockLayout::Div::frac_mask > 0)) {
                    _set_divisor(conf.div_int, conf.div_frac);
                } else {
                    _set_divisor(conf.div_int);
                }
            }
        }

        constexpr std::uint32_t aux_src_val = (static_cast<std::uint32_t>(conf.src) & clk_src_param::ctrl_aux_src_mask_max) >> clk_src_param::ctrl_aux_src_position;

        constexpr std::uint32_t src_val = static_cast<std::uint32_t>(conf.src) & clk_src_param::ctrl_src_mask_max;
        if constexpr (src_val == clk_src_param::ctrl_src_aux_src_val) {
            // if already an auxiliary source is selected, a temporary clock must be selected to prevent glitching when setting the auxiliary source
            if (selected_r::read() == (1U << clk_src_param::ctrl_src_aux_src_val)) {
                constexpr std::uint32_t tmp_clock_src = 0x0U;
                ctrl_r::template Src<tmp_clock_src>::Bits::set(ctrl_r::template Src<tmp_clock_src>::Value::val);

                // wait for the temporary clock to settle
                while (selected_r::selected_bits::getValue() == 0x00U) {}
            }
            ctrl_r::template AuxSrc<aux_src_val>::Bits::set(ctrl_r::template AuxSrc<aux_src_val>::Value::val);
        }
        // The source bit needs to be set in any case
        ctrl_r::template Src<src_val>::Bits::set(ctrl_r::template Src<src_val>::Value::val);

        if constexpr (ClockLayout::Div::int_mask > 0) {
            if constexpr ((ClockLayout::Div::int_mask > 0) && (ClockLayout::Div::frac_mask > 0)) {
                _set_divisor(conf.div_int, conf.div_frac);
            } else {
                _set_divisor(conf.div_int);
            }
        }

        this->m_frequency_hz = conf.frequency_hz;
    }
    template <ClockLayoutConcept ClockLayout>
    template <ClockConfType<ClockLayout> conf>
    std::enable_if_t<(ClockLayout::Ctrl::src_max_val == 0), void> ClockBaseType<ClockLayout>::configure() {
        // If the divisor is increased, the divisor should be set before the source to reduce momentary overspeed.
        // This is according to the RP2040's datasheet.
        if constexpr (ClockLayout::Div::int_mask > 0) {
            constexpr std::uint32_t div_val = []() -> std::uint32_t {
                if constexpr (ClockLayout::Div::frac_mask > 0) {
                    return ((conf.div_int & 0xFFFFFFU) << 8) | conf.div_frac;
                } else {
                    return (conf.div_int & 0xFFFFFFU) << 8;
                }
            }();
            if (div_r::div_reg::read() > (div_val)) {
                if constexpr ((ClockLayout::Div::int_mask > 0) && (ClockLayout::Div::frac_mask > 0)) {
                    _set_divisor(conf.div_int, conf.div_frac);
                } else {
                    _set_divisor(conf.div_int);
                }
            }
        }

        constexpr std::uint32_t aux_src_val = (static_cast<std::uint32_t>(conf.src) & clk_src_param::ctrl_aux_src_mask_max) >> clk_src_param::ctrl_aux_src_position;

        ctrl_r::template Enable<>::Bits::clear();
        /**
         * @brief The expected worst case waiting time in cycles.
         *
         * 2 cycles of the clock sources need to pass for the generated clock to stop. As there is no implementation
         * to get the actual waiting time in cycles, a worst case of a system clock running at 133 MHz and the ROSC
         * running at 6 MHz as the clock source is assumed: 133 MHz / 6MHz = 22.1667 cycles of system clock for one
         * cycle of the source clock. As 2 cycles of the source clock are needed, the value is 45.
         */
        constexpr int expectedWorstCaseCycles {45};
        // Wait for the generated clock to stop
        for (int i = {0}; i < expectedWorstCaseCycles; ++i) {asm volatile("nop");}
        ctrl_r::template AuxSrc<aux_src_val>::set(ctrl_r::template AuxSrc<aux_src_val>::Value::val);
        ctrl_r::template Enable<>::Bits::set(ctrl_r::template Enable<>::Value::val);
        // wait for the generated clock to start
        for (int i = {0}; i < expectedWorstCaseCycles; ++i) {asm volatile("nop");}

        if constexpr (ClockLayout::Div::int_mask > 0) {
            if constexpr ((ClockLayout::Div::int_mask > 0) && (ClockLayout::Div::frac_mask > 0)) {
                _set_divisor(conf.div_int, conf.div_frac);
            } else {
                _set_divisor(conf.div_int);
            }
        }

        this->m_frequency_hz = conf.frequency_hz;
    }

    template <ClockLayoutConcept ClockLayout>
    std::uint32_t ClockBaseType<ClockLayout>::get_frequency_hz() const {
        return this->m_frequency_hz;
    }

    template <ClockLayoutConcept ClockLayout>
    std::enable_if_t<(ClockLayout::Div::int_mask > 0) && (ClockLayout::Div::frac_mask > 0), void>
        ClockBaseType<ClockLayout>::_set_divisor (std::uint32_t integer, std::uint32_t frac) {
        using int_bits = div_r::template Int<integer>;
        int_bits::Bits::set(int_bits::Value::val);
        using frac_bits = div_r::template Frac<frac>;
        frac_bits::Bits::set(frac_bits::Value::val);
    }

    template <ClockLayoutConcept ClockLayout>
    std::enable_if_t<(ClockLayout::Div::int_mask > 0) && (ClockLayout::Div::frac_mask == 0), void>
        ClockBaseType<ClockLayout>::_set_divisor (std::uint32_t integer) {
        using int_bits = div_r::template Int<integer>;
        int_bits::Bits::set(int_bits::Value::val);
    }
}

#endif // RP2040_SYSTEM_CLOCKS_T