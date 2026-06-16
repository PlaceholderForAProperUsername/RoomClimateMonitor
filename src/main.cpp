//
//  on 24.05.26.
//

#include "rp2040/system/xosc.h"


int main() {
    rp2040::system::XOSC::getInstance().init();

    while (true) {}
}