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
    void _exit(int status) {while (1) {}}
    int _close(int file_descriptor) {return -1;}
    int _lseek(int file, int ptr, int dir) {return -1;}
    int _read(int file, char *ptr, int len) {return -1;}
    int _write(int file, char *ptr, int len) {return -1;}

}
