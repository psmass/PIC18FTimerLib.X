# PIC18FTimerLib.X
Basic Timer Lib for PIC18F

Allows you to create logical timers (16) using any one of the 6 physical PIC Timers (TMR0-TMR1) at compile time.
(uncomment the physical Timer in the PIC18F46K22.h file). Timers may each be configured as either a ONE_SHOT or 
CONTINOUS mode timer at timer creation time (new_timer(mode, time-value, callback).  When a timer 'fires' the
callback function is called (note in interrupt context so best to put work on some sort of background work
queue). The timer lib is intended as part of GSM message timeout where a timeout constitutes an error. The physical
selected timer is configured at compile time via #defines vs. runtime to save space (additonally only one set of 
timers can be configured so technically a run time decision is not necessary.

Tuning of time can be done via the #define TMRX_TUNE_VALUE, this is the value loaded each time the PIC TMR 
interrupt is fired and update_timers() is called. If you want to adjust Pre/Post scalers or FOSC division
you will need to play with the code with in the appropriate #ifdef in the init_timer() code.

The PIC MCU is mostly event driven with very limited resources (64K program space, 4K RAM, 1K EEPROM). The code is written to use minimal resources for a real time system with no real operation system (yeah, ouch!). The library footprint isbout 8K of code space and 600 bytes of RAM. BTW - if you remove the assert.h (replace with if() range/error case), the actual lib is only about 4KB and 300B RAM.

Note: the gsm.c file is not needed. The uart.c file is also not part of the library but handy to send messages
as main.c timer callback functions demonstrate.
