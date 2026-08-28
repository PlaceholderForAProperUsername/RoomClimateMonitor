//
//  on 24.05.26.
//


#include "config_sys.h"
#include "rp2040/system/gpio.h"

int main() {
    if (config_system() != 0) {
        return -1;
    }

    while (true) {

    }
}