#include <BOARD.h>
#include <xc.h>
#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "AD.h"
#include "EventChecker.h"
#include "PinsMaster.h"

void main(void)
{
    ES_Return_t ErrorType;

    BOARD_Init();
    AD_Init();
    PWM_Init();
    RC_Init();

    printf("Starting ES Framework Template\r\n");
    printf("using the 2nd Generation Events & Services Framework\r\n");


    // Your hardware initialization function calls go here
    
    // AD Pin Initialization
    AD_AddPins(AD_PIN_LIST);
    // PWM Pin Initialization
    PWM_AddPins(PWM_PIN_LIST);
    // RC Pin Initialization
    RC_AddPins(RC_PIN_LIST);
    RC_SetPulseTime(SERVO_0_OUT_PIN,1000);
    RC_SetPulseTime(SERVO_1_OUT_PIN,1000);
    
    

    // now initialize the Events and Services Framework and start it running
    ErrorType = ES_Initialize();
    if (ErrorType == Success) {
        ErrorType = ES_Run();

    }
    //if we got to here, there was an error
    switch (ErrorType) {
    case FailedPointer:
        printf("Failed on NULL pointer");
        break;
    case FailedInit:
        printf("Failed Initialization");
        break;
    default:
        printf("Other Failure: %d", ErrorType);
        break;
    }
    for (;;)
        ;

};

/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/
