#include "xc.h"
#include <BOARD.h>
#include <stdio.h>
#include <string.h>
#include <AD.h>
#include <RC_Servo.h>
#include <LED.h>
#include <pwm.h>
#include <IO_Ports.h>
#include <PinsMaster.h>

#define WHITE_TAPE_THRESHOLD 800
#define BLACK_TAPE_THRESHOLD 400

#define TEST_PIN AD_PORTV7

void LED_meter (unsigned int pot);

int main(void) {
    BOARD_Init();
    SERIAL_Init();
    AD_Init();
    PWM_Init();
    LED_Init();
    TIMERS_Init();
    TIMERS_InitTimer(1, 500);

    AD_AddPins(AD_PIN_LIST);
    IO_PortsSetPortOutputs(PORTW, PIN4);
    PWM_AddPins(PWM_PIN_LIST);
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);
    

    
    //Test Code//
    
    while (1) {
        if (AD_IsNewDataReady() == TRUE && TIMERS_IsTimerExpired(1)) {
            // Potentiometer
            unsigned int sample1 = AD_ReadADPin(TEST_PIN);
            
            printf("|%d|\n", sample1);
            TIMERS_InitTimer(1, 500);
        }
        
        uint8_t input = GetChar();
        
        switch(input){
            // Add cases here
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