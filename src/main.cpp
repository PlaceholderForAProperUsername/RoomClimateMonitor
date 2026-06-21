//
//  on 24.05.26.
//

#include "rp2040/system/xosc.h"
#include "rp2040/system/resets.h"


int main() {
    rp2040::system::XOSC::getInstance().init();
    rp2040::system::Resets::getInstance().enable<rp2040::system::SubsystemBits::SPI1>();


    while (true) {}
}