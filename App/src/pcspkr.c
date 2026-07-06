#include <pcspkr.h>




void pcspkr_tone(uint64_t hz){
    if(hz == 0){
        return;
    }

    uint64_t divisor = 1193182 / hz;

    //PIT channel 2, square wave
    outb(0x43, 0xB6);

    //low byte
    outb(0x42, divisor & 0xFF);

    //high byte
    outb(0x42, divisor >> 8);

    //enable speaker
    uint8_t tmp = inb(0x61);

    outb(0x61, tmp | 0x03);
}

void pcspkr_stop(){
    uint8_t tmp = inb(0x61);
    outb(0x61, tmp & ~0x02);
}






