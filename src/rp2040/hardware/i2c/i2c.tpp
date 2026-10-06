/**
 * @file i2c.tpp
 * @brief The I2C module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 02.10.2026
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

#ifndef RP2040_HARDWARE_I2C_T
#define RP2040_HARDWARE_I2C_T

#include <utility>

#include "include/rp2040/hardware/i2c.h"
#include "rp2040/system/resets.h"

namespace rp2040::hardware::i2c {
    /**
     * @addtogroup rp2040_i2c
     * @{
     */

    template <std::uintptr_t base>
    I2cType<base>& I2cType<base>::get_instance() {
        static I2cType instance;
        return instance;
    }

    template <std::uintptr_t base>
    void I2cType<base>::init() {
        if (m_flags.test(std::to_underlying(Flags::initialized))) {
            return;
        }
        if constexpr (base == address::i2c0_base) {
            system::resets::Resets::get_instance().reset<system::resets::Subsystems::i2c0>();
        } else if constexpr (base == address::i2c1_base) {
            system::resets::Resets::get_instance().reset<system::resets::Subsystems::i2c1>();
        }

        m_flags.set(std::to_underlying(Flags::initialized));
        m_flags.reset(std::to_underlying(Flags::configured));
    }

    template <std::uintptr_t base>
    void I2cType<base>::deinit() {
        if constexpr (base == address::i2c0_base) {
            system::resets::Resets::get_instance().disable<system::resets::Subsystems::i2c0>();
        } else if constexpr (base == address::i2c1_base) {
            system::resets::Resets::get_instance().disable<system::resets::Subsystems::i2c1>();
        }

        m_flags.reset();
    }

    template <std::uintptr_t base>
    template <MasterConfig config>
    status::Status I2cType<base>::configure() {
        // static_assert can't be used in the constructor of MasterConfig, because the parameters are not inherently considered constexpr.
        if constexpr (config.speed_mode == SpeedMode::standard) {
            static_assert(config.sys_clock_hz >= 2'700'000, "Minimum system frequency for standard mode is 2.7MHz!");
        } else if constexpr (config.speed_mode == SpeedMode::fast) {
            static_assert(config.sys_clock_hz >= 12'000'000, "Minimum system frequency for standard mode is 12MHz!");
        } else if constexpr (config.speed_mode == SpeedMode::fast_plus) {
            static_assert(config.sys_clock_hz >= 32'000'000, "Minimum system frequency for standard mode is 32MHz!");
        }
        static_assert(config.sys_clock_hz <= system::pll::limits::pll_sys_max_freq_hz, "The rp2040 supports a maximum system frequency of 133MHz (pll_sys)!");
        if (!m_flags.test(std::to_underlying(Flags::initialized))) {
            return status::Status::ERROR_INIT;
        }
        // make sure i2c controller is disabled, otherwise some registers cannot be configured.
        Registers::IcEnable::Enable::Bits::clear();
        static constexpr std::uint32_t min_scl_low_time_ns = [] () {
            if constexpr (config.speed_mode == SpeedMode::standard) {
                return 4'700;
            } else if constexpr (config.speed_mode == SpeedMode::fast) {
                return 1'300;
            } else if constexpr (config.speed_mode == SpeedMode::fast_plus) {
                return 500;
            }
        } ();
        static constexpr std::uint32_t min_scl_high_time_ns = [] () {
            if constexpr (config.speed_mode == SpeedMode::standard) {
                return 4'000;
            } else if constexpr (config.speed_mode == SpeedMode::fast) {
                return 600;
            } else if constexpr (config.speed_mode == SpeedMode::fast_plus) {
                return 260;
            }
        } ();
        static constexpr std::uint32_t low_count = min_scl_low_time_ns * config.sys_clock_hz;
        static constexpr std::uint32_t high_count = min_scl_high_time_ns * config.sys_clock_hz;
        static constexpr std::uint32_t sda_tx_hold_count = [] () {
            if constexpr ((config.speed_mode == SpeedMode::standard) || (config.speed_mode == SpeedMode::fast)) {
                return (((config.sys_clock_hz * 3) / 10'000'000) + 1);
            } else {
                return (((config.sys_clock_hz * 3) / 25'000'000) + 1);
            }
        } ();
        static_assert(sda_tx_hold_count <= (low_count - 2));

        Registers::Ctrl::IcSlaveDisable::Bits::set(Registers::Ctrl::IcSlaveDisable::Value::slave_disabled);
        Registers::Ctrl::Ic10BitAddrMaster::Bits::set(config.address_mode);
        Registers::Ctrl::Speed::Bits::set(config.speed_mode);
        Registers::Ctrl::MasterMode::Bits::set(Registers::Ctrl::MasterMode::Value::enable);

        if constexpr (config.speed_mode == SpeedMode::standard) {
            using LowCountBits = Registers::SsSclLcnt::template Cnt<low_count>;
            LowCountBits::Bits::set(LowCountBits::Value::val);
            using HighCountBits = Registers::SsSclHcnt::template Cnt<high_count>;
            HighCountBits::Bits::set(HighCountBits::Value::val);
        } else {
            using LowCountBits = Registers::FsSclLcnt::template Cnt<low_count>;
            LowCountBits::Bits::set(LowCountBits::Value::val);
            using HighCountBits = Registers::FsSclHcnt::template Cnt<high_count>;
            HighCountBits::Bits::set(HighCountBits::Value::val);
        }

        using SdaTxHoldBits = Registers::SdaHold::template SdaTxHold<sda_tx_hold_count>;
        SdaTxHoldBits::Bits::set(SdaTxHoldBits::Value::val);

        static constexpr std::uint32_t spike_length = (low_count < 16) ? 1 : low_count / 16;

        using SpikeLengthBits = Registers::IcFsSpkLen::template SpkLen<spike_length>;
        SpikeLengthBits::Bits::set(SpikeLengthBits::Value::val);

        Registers::IcEnable::Enable::Bits::set(Registers::IcEnable::Enable::Value::enable);

        return status::Status::OK;
    }

    template <std::uintptr_t base>
    template <std::uint16_t target_addr>
    status::Status I2cType<base>::write(const std::span<std::uint8_t> data, const bool send_stop) {
        if (!m_flags.test(std::to_underlying(Flags::configured))) {
            return status::Status::ERROR_INIT;
        }
        using StatusReg = Registers::Status;
        using TargetBits = Registers::Tar::template IcTar<target_addr>;
        // Checks if target address in hardware differs from new target address
        if (TargetBits::Bits::get_value() != TargetBits::Value::val) {
            // wait for active transmissions to end
            while (StatusReg::Activity::Bits::get_value() == StatusReg::Activity::Value::active) {}
            using EnableBits = Registers::IcEnable::Enable;
            EnableBits::Bits::clear(); // disabling I2C controller necessary to set new target address
            TargetBits::Bits::set(TargetBits::Value::val);
            EnableBits::Bits::set(EnableBits::Value::enable);
        }
        for (size_t i {0}; i < data.size(); ++i) {
            // wait until there is space in the transmit fifo
            while (StatusReg::Tfnf::Bits::get_value() == StatusReg::Tfnf::Value::full) {}
            using DataCmdBits = Registers::DataCmd::DataCommand;
            auto val = DataCmdBits::Value(data[i], DataCmdBits::Flags::write_cmd | (send_stop && (i == (data.size() - 1)) ? DataCmdBits::Flags::send_stop : 0));
            DataCmdBits::Bits::set(val);
        }
        return status::Status::OK;
    }

    /** @} */ // rp2040_i2c
}

#endif //RP2040_HARDWARE_I2C_T