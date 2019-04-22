/* 
 * File:   timerLib.h
 * Author: pschmitt
 *
 * Created on April 21, 2019, 7:15 AM
 * 
 * Header file for timer API
 */

extern enum tmrMode {ONE_SHOT, CONTINUOUS};
extern void init_timer(); 
extern int new_timer(enum tmrMode mode, int time, void (*callback)(int)) ;
extern void delete_timer(int indx);
