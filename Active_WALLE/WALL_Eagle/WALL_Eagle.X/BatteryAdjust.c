/* 
 * File:   BatteryAdjust.c
 * Author: jsgier
 *
 * Single Function BatteryAdjust.c. Input a speed from 0-1000, representing a range
 * of possible output voltages from 0 to 6.5 (reduced from 0 to 9.9 total range
 * so that PWM has more range to reduce before voltage changes). 
 * 
 * Created on November 9, 2016, 4:57 PM
 */

#include "xc.h"
#include <BOARD.h>
#include <stdio.h>
#include <AD.h>
#include <pwm.h>
#include <IO_Ports.h>
#include <BatteryAdjust.h>
#include <ES_Configure.h>

#define MAX_VOLTAGE 9.7		//max battery voltage, should be reduced from absolute maximum from battery	
							//to increase the range of the PWM. Eg. if you put in a battery voltage of 
                            // 5.0V, the duty cycle can be adjusted to maintain this max for longer than 
                            // a battery voltage of 9.0V



int AdjustPWM(int SpeedInputPWM){
    int SpeedOutputPWM;
    SpeedOutputPWM = SpeedInputPWM;
    
#ifdef USE_BAT_ADJUSTED_SPEED
    int BatteryVoltage;
    double PercentDutyCycle;

    BatteryVoltage = AD_ReadADPin(BAT_VOLTAGE);

    PercentDutyCycle = MAX_VOLTAGE/(((BatteryVoltage/1023)*33)-0.6);
        
    SpeedOutputPWM = (int)(SpeedInputPWM * PercentDutyCycle);
    
#endif
    
    return SpeedOutputPWM;
            
}



