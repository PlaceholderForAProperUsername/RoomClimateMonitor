//
//  on 24.05.26.
//


#include "config_sys.h"
#include "rp2040/system/gpio.h"

int main() {
    if (config_system() != 0) {
        return -1;
    }

    rp2040::system::gpio::GPIO::getInstance().init();

    rp2040::system::gpio::GPIO::getInstance().set_function<rp2040::system::gpio::Pins::GPIO0, rp2040::system::gpio::FuncSelect::SIO>();

    rp2040::system::gpio::GPIO::getInstance().enable_output<rp2040::system::gpio::Pins::GPIO0, true>();

    rp2040::system::gpio::GPIO::getInstance().toggle_output<rp2040::system::gpio::Pins::GPIO0>();

    rp2040::system::gpio::GPIO::getInstance().toggle_output<rp2040::system::gpio::Pins::GPIO0>();

    rp2040::system::gpio::GPIO::getInstance().enable_pull_up<rp2040::system::gpio::Pins::GPIO0, true>();

    rp2040::system::gpio::GPIO::getInstance().enable_pull_down<rp2040::system::gpio::Pins::GPIO0, false>();

    rp2040::system::gpio::GPIO::getInstance().enable_pull_down<rp2040::system::gpio::Pins::GPIO0, true>();

    rp2040::system::gpio::GPIO::getInstance().enable_pull_up<rp2040::system::gpio::Pins::GPIO0, false>();

    while (true) {

    }
}