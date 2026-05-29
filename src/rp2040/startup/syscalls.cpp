//
//  on 26.05.26.
//

#include <iterator>

extern  "C" {

    extern unsigned int _start_heap;

    void *_sbrk(unsigned int incr) {
        static unsigned char *heap = (unsigned char *) &_start_heap;
        void *prev_heap = heap;
        // check if incr is word aligned
        if ((incr & 0x03) != 0) {
            incr = ((incr >> 2) + 1) << 2;
        }
        heap += incr;
        return prev_heap;
    };

    // unused stubs for nanolib
    void _exit([[maybe_unused]] int status) {while (1) {}}
    int _close([[maybe_unused]] int file_descriptor) {return -1;}
    int _lseek([[maybe_unused]] int file, [[maybe_unused]] int ptr, [[maybe_unused]] int dir) {return -1;}
    int _read([[maybe_unused]] int file, [[maybe_unused]] char *ptr, [[maybe_unused]] int len) {return -1;}
    int _write([[maybe_unused]] int file, [[maybe_unused]] char *ptr, [[maybe_unused]] int len) {return -1;}

}   // extern "C"
