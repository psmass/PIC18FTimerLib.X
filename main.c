/*
 * File:   main.c
 * Author: pschmitt
 *
 * Created on April 17, 2019, 10:16 AM
 */

#include <xc.h>
#include <stdio.h>
#include "timerLib.h"
#include "PIC18F46K22.h"

extern void init_uart(void);
extern void putch(unsigned char data);

void timerCallbackFuncOneShot(int indx);
void timerCallbackFuncCont(int indx);

void main(void) {
    init_uart();
    init_timer();
    int t1=0;
    
    /* test code for max number of timers 
    #define NUM_TIMERS 16
    
    int myTimer[NUM_TIMERS]; 
    
    // i<=NUM_TIMERS to assert on no timers available
    for (int i=0; i<NUM_TIMERS; i++)
        myTimer[i] = new_timer(10+i, timerCallbackFunc);
    */
    
    int myTimer = new_timer(CONTINUOUS, 5, timerCallbackFuncCont);
    int myOneShotTimer = new_timer(ONE_SHOT, 20, timerCallbackFuncOneShot);
    
    while(1) {
      
    }
}

void timerCallbackFuncOneShot(int indx) {
    printf("One-shot Timer %-2d  Expired\n", indx);
    delete_timer(indx); 
}

void timerCallbackFuncCont(int indx) {
    printf("Continuous Timer %-2d  Ticked\n", indx);
}

