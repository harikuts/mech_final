#include "xc.h"
#include <BOARD.h>
#include <stdio.h>
#include <string.h>
#include <AD.h>
#include <RC_Servo.h>
#include <LED.h>
#include <pwm.h>
#include <IO_Ports.h>
#include <motors.h>

#define TEST_PWM_PIN RC_PORTY06
#define TEST_DIR_PIN PORTY,PIN3

#define PWM_FREQUENCY 1000

void LED_meter (unsigned int pot);

int main(void) {
    BOARD_Init();
    SERIAL_Init();
    AD_Init();
    PWM_Init();
    LED_Init();
    RC_Init();
    TIMERS_Init();
    TIMERS_InitTimer(1, 500);
    InitMotors();
    
    int freq = PWM_FREQUENCY;
    
    RC_AddPins(TEST_PWM_PIN); 

    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);
    
    PWM_SetFrequency(PWM_FREQUENCY);
    
    while (1) {
        char input = GetChar();
        int cur_freq = PWM_GetFrequency();
        int cur_dc = PWM_GetDutyCycle(TEST_PWM_PIN);
        switch (input){
            case 'h':
                RC_SetPulseTime(TEST_PWM_PIN, 1000);
                printf("|CW|", PWM_GetFrequency());
                break;
            case 'j':
                RC_SetPulseTime(TEST_PWM_PIN, 2000);
                printf("|CCW|", PWM_GetFrequency());
                break;
        }
    } 
    
    return 0;
}

void LED_meter (unsigned int pot) {
    float div = 1024 / 13;
    unsigned char first_bank = (pot > div) | ((pot > div * 2) << 1) | ((pot > div * 3) << 2) | ((pot > div * 4) << 3);
    unsigned char second_bank = (pot > div * 5) | ((pot > div * 6) << 1) | ((pot > div * 7) << 2) | ((pot > div * 8) << 3);
    unsigned char third_bank = (pot > div * 9) | ((pot > div * 10) << 1) | ((pot > div * 11) << 2) | ((pot > div * 12) << 3);
    
    LED_SetBank(LED_BANK1, first_bank);
    LED_SetBank(LED_BANK2, second_bank);
    LED_SetBank(LED_BANK3, third_bank);
}