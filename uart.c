/* ***************************************************************************
 * UART Routines
 */

#include <xc.h>

void init_uart(void) {
    TXSTAbits.TXEN = 1;               // enable transmitter
    RCSTAbits.SPEN = 1;               // enable serial port
}

void putch(unsigned char data) {
    while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
        continue;
    TXREG = data;                     // send one character
}
