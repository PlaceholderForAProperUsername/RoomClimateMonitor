/**
 * @file config_sys.cpp
 * @brief Initializes main components and configures the clocks of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 25.07.2026
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


#include "config_sys.h"
#include "rp2040/system/clocks.h"
#include "rp2040/system/xosc.h"
#include "rp2040/system/pll.h"

int config_system() {
    rp2040::system::XOSC::getInstance().init();
    constexpr rp2040::system::clocks::SysClockConfig sysXoscConf {rp2040::system::clocks::SysClock_DefType::ClockSrc::XOSC, 12'000'000};
    rp2040::system::clocks::SysClock::getInstance().configureClock<sysXoscConf>();
    rp2040::system::pll::PLL_Sys::getInstance().init();
    constexpr rp2040::system::pll::PLL_ConfigType config {};
    if (!rp2040::system::pll::PLL_Sys::getInstance().setFrequency<config>().has_value()) {
        return -1;
    }
    constexpr rp2040::system::clocks::RefClockConfig refConf {rp2040::system::clocks::RefClock_DefType::ClockSrc::XOSC, 12'000'000};
    rp2040::system::clocks::RefClock::getInstance().configureClock<refConf>();
    constexpr rp2040::system::clocks::SysClockConfig sysConf {rp2040::system::clocks::SysClock_DefType::ClockSrc::PLL_SYS, 125'000'000};
    rp2040::system::clocks::SysClock::getInstance().configureClock<sysConf>();
    constexpr rp2040::system::clocks::PeriClockConfig periConf {rp2040::system::clocks::PeriClock_DefType::ClockSrc::CLK_SYS, 125'000'000};
    rp2040::system::clocks::PeriClock::getInstance().configureClock<periConf>();

    return 0;
}