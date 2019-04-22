/* 
 * File:   timerLib.c
 * Author: pschmitt
 *
 * Created on April 21, 2019, 7:15 AM
 * 
 * Code leveraged from http://stevephillips.me/blog/pic18-basics-timers
 * 
 * This function allows any of timers T0, T2, T3, T6 to be used as
 * a source for this library. The library is currently configured as
 * follows for the various clocks assuming 8MHz _XTAL_FREQ:
 *      TMR0 - 1 sec interrupt . 
 *      TMR1, TMR3, TMR5 - ~1/4sec interrupt (1/(FOSC/4) * 1:8 prescale * 65535)
 * 
 *      TMR2, TMR4, TMR6 - .032sec interrupt (1/(FOSC/4)) * 16Prescale * 
 *                          16Post * 256 = .032 Sec timeout
 * 
 *  Notes: (1) TMR2/4/6 are not recommended due to the high overhead
 *         (2) The Interrupt 
 * 
 * 
 */


// include the XC and PIC18 libraries
#include <xc.h>
#include <pic18.h>
#include <assert.h>
#include <stdbool.h>
#include "PIC18F46K22.h"

#define MAX_TIMERS 16
#define TIMED_OUT 0

// sets the selected Timer Val to reload dependent upon TIMERn selected in PICxxxx.h.
#ifdef TIMER0
    #define TMRX TMR0
    #define TMRX_TUNE_VAL 50271
    #define TMRXIE TMR0IE
#endif
#ifdef TIMER1
    #define TMRX TMR1
    #define TMRX_TUNE_VAL 0
    #define TMRXIE TMR1IE
#endif
#ifdef TIMER2
    #define TMRX TMR2
    #define TMRX_TUNE_VAL 0
    #define TMRXIE TMR2IE
#endif
#ifdef TIMER3
    #define TMRX TMR3
    #define TMRX_TUNE_VAL 0
    #define TMRXIE TMR3IE
#endif
#ifdef TIMER4
    #define TMRX TMR4
    #define TMRX_TUNE_VAL 0
    #define TMRXIE TMR4IE
#endif
#ifdef TIMER5
    #define TMRX TMR5
    #define TMRX_TUNE_VAL 0
    #define TMRXIE TMR5IE
#endif
#ifdef TIMER6
    #define TMRX TMR5
    #define TMRX_TUNE_VAL 0
    #define TMRXIE TMR6IE
#endif

enum tmrMode {ONE_SHOT, CONTINUOUS};

unsigned long int mainTimer = 0;

struct timer {
    unsigned long int timeSetVal; // timer val used to reset time if CONT mode
    unsigned long int time;
    void (* callback)(int);
    _Bool timerAvailable;
    enum tmrMode mode;
} timers[MAX_TIMERS]; // array of timer

//  main loop of the program
void init_timer() {
    // initialize timers to NULL
    for (int i=0; i<MAX_TIMERS; i++)
        timers[i].timerAvailable=true;
    
#ifdef TIMER0 // if TMR0 is underlying physical timer
    //16 bit mode
        T0CONbits.T08BIT = 0;
        
        // Select internal Clock source
        T0CONbits.T0CS=0;

        // enable prescaler for Timer 0
        T0CONbits.PSA = 0;

        // prescale TMR0 to 1:128
        T0CONbits.T0PS = 0b110;

        // enable Timer 0, part of the T0CON register
        T0CONbits.TMR0ON=1;
        //TMR0ON = 1;

        // alternatively, you can set the previous commands in one line, like this:
        //T0CON = 0b10000110;

#endif
// If Timer T1/T3/ or T5 is physical underlying timer 
#ifdef TIMER1
    //T1/T3/T5 are 16 bit 
    T1CONbits.T1RD16=1;

    // Select internal Clock source
    T1CONbits.TMR1CS=0;
    T1CONbits.T1SOSCEN=0;

    // enable prescaler for Timer 1:8
    T1CONbits.T1CKPS=0b11;

    // Timer counts regardless of gate function
    T1GCONbits.TMR1GE=0;

    // enable Timer, part of the T0CON register
    T1CONbits.TMR1ON=1;
#endif   
#ifdef TIMER3
    //T1/T3/T5 are 16 bit 
    T3CONbits.T3RD16=1;

    // Select internal Clock source
    T3CONbits.TMR3CS=0;
    T3CONbits.T3SOSCEN=0;

    // enable prescaler for Timer 1:8
    T3CONbits.T3CKPS=0b11;

    // Timer counts regardless of gate function
    T3GCONbits.TMR3GE=0;

    // enable Timer, part of the T0CON register
    T3CONbits.TMR3ON=1;
#endif 
#ifdef TIMER5
    //T1/T3/T5 are 16 bit 
    T5CONbits.T5RD16=1;

    // Select internal Clock source
    T5CONbits.TMR5CS=0;
    T5CONbits.T5SOSCEN=0;

    // enable prescaler for Timer 1:8
    T5CONbits.T5CKPS=0b11;

    // Timer counts regardless of gate function
    T5GCONbits.TMR5GE=0;

    // enable Timer, part of the T0CON register
    T5CONbits.TMR5ON=1;
#endif
// If Timer 2/4/ or 6 as physical underlying Timer
#ifdef TIMER2
    //T2/T4/T6 are 8 bit timers 
    //Postscale 1:16
    T2CONbits.T2OUTPS=0b1111;

    //Prescale 16 0b1x
    T2CONbits.T2CKPS=0b11;

    // enable Timer, part of the T0CON register
    T2CONbits.TMR2ON=1;
#endif
#ifdef TIMER4
    //T2/T4/T6 are 8 bit timers 
    //Postscale 1:16
    T4CONbits.T4OUTPS=0b1111;

    //Prescale 16 0b1x
    T4CONbits.T4CKPS=0b11;

    // enable Timer, part of the T0CON register
    T4CONbits.TMR4ON=1;
#endif
#ifdef TIMER6
    //T2/T4/T6 are 8 bit timers 
    //Postscale 1:16
    T6CONbits.T6OUTPS=0b1111;

    //Prescale 16 0b1x
    T6CONbits.T6CKPS=0b11;

    // enable Timer, part of the T0CON register
    T6CONbits.TMR6ON=1;
#endif 
    // Set Timer X interrupt enable
    TMRXIE=1;
    // enable general interrupts
    PEIE = 1;
    GIE = 1;
    
    // and you can set those last 2 in one line, like this:
    // INTCON = 0b10100000;
    return;
}

int new_timer(enum tmrMode mode, int time, void (*callback)(int)) {
    // Takes a callback function pointer to a function taking a timer index
    // and returning void. The function is expected to handle a time out and
    // MUST return the timer to pool via a delete_timer(timerIndex) call.
    // Returns a timer index for later deletion
    int i;
    for (i=0; i<MAX_TIMERS; i++)
        if (timers[i].timerAvailable) {
            timers[i].timeSetVal=time;
            timers[i].time=time;
            timers[i].callback=callback;
            timers[i].timerAvailable=false;
            timers[i].mode=mode;
            break;
        }
    assert (i<MAX_TIMERS); // out of timers should never happen
    return i;
}

void update_timers() {
    // This routine runs in the context of the timer interrupt
    // It updates all active timers and performs the callback
    // if the timer expires. The callback is expected to be brief.
    

    TMRX = TMRX_TUNE_VAL;
     
    for (int i=0; i<MAX_TIMERS; i++)
        if (!timers[i].timerAvailable && timers[i].time !=TIMED_OUT) {
           timers[i].time--;
           if (timers[i].time==TIMED_OUT) {
               //printf("In update Timer %d expired\n", i);
               timers[i].callback(i);
               if (timers[i].mode==CONTINUOUS) {
                   timers[i].time=timers[i].timeSetVal;
               }
           }
        }
}

void delete_timer(int indx) {
    assert(0 <= indx);   // Attempt to delete a timer out of range
    assert(indx < MAX_TIMERS);
    assert(timers[indx].timerAvailable==false); // attempt to delete a timer not in use
    timers[indx].timerAvailable=true; // cancels timer and returns it to pool.
    //printf("timer %d deleted", indx);
}