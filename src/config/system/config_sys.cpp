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
    constexpr rp2040::system::clocks::SysClockConfig sys_rosc_conf {rp2040::system::clocks::SysClock_DefType::ClockSrc::ROSC, 6'000'000};
    rp2040::system::clocks::SysClock::getInstance().configureClock<sys_rosc_conf>();

    rp2040::system::xosc::XOSC::get_instance().init();
    // setting the xosc temporarily as the system clock is to prevent a hung up after soft reset where the system clock is still configured to use the pll and configuring the pll causes the pll to stop.
    constexpr rp2040::system::clocks::SysClockConfig sys_xosc_conf {rp2040::system::clocks::SysClock_DefType::ClockSrc::XOSC, 12'000'000};
    rp2040::system::clocks::SysClock::getInstance().configureClock<sys_xosc_conf>();
    constexpr rp2040::system::clocks::RefClockConfig ref_conf {rp2040::system::clocks::RefClock_DefType::ClockSrc::XOSC, 12'000'000};
    rp2040::system::clocks::RefClock::getInstance().configureClock<ref_conf>();

    rp2040::system::pll::PllSys::get_instance().init();
    constexpr rp2040::system::pll::PllConfigType config {1U, 6U, 2U, 125U};
    if (!rp2040::system::pll::PllSys::get_instance().set_frequency<config>().has_value()) {
        return -1;
    }
    constexpr rp2040::system::clocks::SysClockConfig sys_pll_conf {rp2040::system::clocks::SysClock_DefType::ClockSrc::PLL_SYS, 125'000'000};
    rp2040::system::clocks::SysClock::getInstance().configureClock<sys_pll_conf>();
    constexpr rp2040::system::clocks::PeriClockConfig peri_conf {rp2040::system::clocks::PeriClock_DefType::ClockSrc::CLK_SYS, 125'000'000};
    rp2040::system::clocks::PeriClock::getInstance().configureClock<peri_conf>();
    return 0;
}