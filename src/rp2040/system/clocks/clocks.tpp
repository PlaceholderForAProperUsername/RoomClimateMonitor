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
#include "rp2040/system/clocks_def.h"

namespace rp2040::system::clocks {

    template <typename ClockDefType>
    requires IsClockType<ClockDefType>
    ClockBaseType<ClockDefType>& ClockBaseType<ClockDefType>::getInstance() {
        static ClockBaseType<ClockDefType> instance;
        return instance;
    }

    template <typename ClockDefType>
    requires IsClockType<ClockDefType>
    template <ClockConfType<ClockDefType> conf>
    void ClockBaseType<ClockDefType>::configureClock() {

        // If the divisor is increased, the divisor should be set before the source to reduce momentary overspeed.
        // This is according to the RP2040's datasheet.
        if constexpr (HasDivRegister<ClockDefType>) {
            constexpr std::uint32_t divVal = []() -> std::uint32_t {
                if constexpr (HasDivWithFracRegister<ClockDefType>) {
                    return ((conf.div_int & 0xFFFFFFU) << 8) | conf.div_frac;
                } else {
                    return (conf.div_int & 0xFFFFFFU) << 8;
                }
            }();
            if (div_r::div_reg::read() > (divVal)) {
                div_r::template int_bits<conf.div_int>::set(div_r::template IntBitField<conf.div_int>::value::val);
                if constexpr (HasDivWithFracRegister<ClockDefType>) {
                    div_r::template frac_bits<conf.div_frac>::set(div_r::template FracBitField<conf.div_frac>::value::val);
                }
            }
        }

        constexpr std::uint32_t auxSrcVal = (static_cast<std::uint32_t>(conf.src) & ctrlAuxSrcMask_max) >> ctrlAuxSrcPosition;

        if constexpr (ClockHasGlitchlessSrcType<ClockDefType>) {
            constexpr std::uint32_t srcVal = static_cast<std::uint32_t>(conf.src) & ctrlSrcMask_max;
            if constexpr (srcVal == ctrlSrcAuxSrcBit) {
                // if already an auxiliary source is selected, a temporary clock must be selected to prevent glitching when setting the auxiliary source
                if (selected_r::selected_bits::getValue() == static_cast<std::uint32_t>(selected_r::SelectedBitField::value::AUX)) {
                    constexpr std::uint32_t tmpClockSrc = 0x0U;
                    ctrl_r::template src_bits<tmpClockSrc>::set(ctrl_r::template SrcBitField<tmpClockSrc>::value::val);

                    // wait for the temporary clock to settle
                    while (selected_r::selected_bits::getValue() == 0x00U) {}
                }
                ctrl_r::template aux_src_bits<auxSrcVal>::set(ctrl_r::template AuxSrcBitField<auxSrcVal>::value::val);
                ctrl_r::template src_bits<srcVal>::set(ctrl_r::template SrcBitField<srcVal>::value::val);

                while (selected_r::selected_bits::getValue() != static_cast<std::uint32_t>(selected_r::SelectedBitField::value::AUX)) {}

            } else {
                // As the source is not an auxiliary source, the source can be set directly
                ctrl_r::template src_bits<srcVal>::set(ctrl_r::template SrcBitField<srcVal>::value::val);
            }
        } else {
            ctrl_r::enable_bits::clear();

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
            for (int i = {0}; i < expectedWorstCaseCycles; ++i) {}
            ctrl_r::template aux_src_bits<auxSrcVal>::set(ctrl_r::template AuxSrcBitField<auxSrcVal>::value::val);
            ctrl_r::enable_bits::set(ctrl_r::enable_bits::value::enable);
            // wait for the generated clock to start
            for (int i = {0}; i < expectedWorstCaseCycles; ++i) {}
        }

        if constexpr (HasDivRegister<ClockDefType>) {
            div_r::template int_bits<conf.div_int>::set(div_r::template IntBitField<conf.div_int>::value::val);
            if constexpr (HasDivWithFracRegister<ClockDefType>) {
                div_r::template frac_bits<conf.div_frac>::set(div_r::template FracBitField<conf.div_frac>::value::val);
            }
        }

        this->m_frequency_hz = conf.frequency_hz;
    }

    template <typename ClockDefType>
    requires IsClockType<ClockDefType>
    std::uint32_t ClockBaseType<ClockDefType>::getFrequencyHz() {
        return this->m_frequency_hz;
    }
}

#endif // RP2040_SYSTEM_CLOCKS_T_H