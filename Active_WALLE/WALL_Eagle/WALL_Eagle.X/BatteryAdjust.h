/* 
 * File:   BatteryAdjust.h
 * Author: jsgier
 *
 * Single Function BatteryAdjust.c. Input a speed from 0-1000, representing a desired
 * speed in reduced range (reduced from 0 to 9.9 total range
 * so that PWM has more range to reduce before voltage changes). Returns a PWM from 
 * 0 to 1000 depending on battery voltage, can be directly output to motors
 * 
 * Created on November 9, 2016, 4:57 PM
 */


#ifndef BATTERYADJUST_H
#define	BATTERYADJUST_H

#ifdef	__cplusplus
extern "C" {
#endif


    int AdjustPWM(int SpeedInput);
    

#ifdef	__cplusplus
}
#endif

#endif	/* BATTERADJUST_H */

