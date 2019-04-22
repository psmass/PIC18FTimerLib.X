#include <xc.h>
#include <pic18.h>
#include "PIC18F46K22.h"

// timer that will increment 4 times per second
extern unsigned long int mainTimer;
extern void update_timers();

// interrupt service routine to handle all interrupts
void __interrupt() main_isr(void) {
    // if Timer  interrupt flag has been raised
    if (TMRXIF) {
        // increment the seconds counter
        mainTimer++;
        update_timers();

        // lower the interrupt flag
        TMRXIF = 0;
    }
}

