
/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "BOARD.h"
#include "AD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BumperService.h"
#include "string.h"
#include "ES_Timers.h"
#include "IO_Ports.h"
#include <stdio.h>
#include "PinsMaster.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
//#define LEFT_BUMPER_PIN AD_PORTV3
//#define RIGHT_BUMPER_PIN AD_PORTV7

#define HIGH_THRESHOLD 800
#define LOW_THRESHOLD 200

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/

/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */
typedef enum {
    IDLE,
    DEPRESSED,
} bumper_state;


/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
/* You will need MyPriority and maybe a state variable; you may need others
 * as well. */

static uint8_t MyPriority;
uint8_t old_state = 0;
uint8_t new_state = 0;
char output[32];

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t InitBumperService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;

    
    //AD_AddPins(AD_ActivePins() | LEFT_BUMPER_PIN | RIGHT_BUMPER_PIN);

    // Start the timer and let it run expiring every 2 ms
    ES_Timer_InitTimer(BUMPER_TIMER, 25);

    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

uint8_t PostBumperService(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

ES_Event RunBumperService(ES_Event ThisEvent) {
    ES_Event ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors

    /********************************************
     in here you write your service code
     *******************************************/
    static ES_EventTyp_t lastEvent = WHITE_TAPE_DETECTED;
    ES_EventTyp_t curEvent;

    switch (ThisEvent.EventType) {
        case ES_TIMERACTIVE:
        case ES_INIT:
            // No hardware initialization or single time setups, those
            // go in the init function above.
            //
            // This section is used to reset service for some reason
            break;

        case ES_TIMEOUT:
            //printf("*");
            ES_Timer_InitTimer(BUMPER_TIMER, 25);
            if (AD_IsNewDataReady()) {
                uint16_t left_bumper_val = AD_ReadADPin(LEFT_BUMPER_PIN);
                uint16_t right_bumper_val = AD_ReadADPin(RIGHT_BUMPER_PIN);
                //printf("|\tR: %d\tL: %d\t|\n", right_bumper_val, left_bumper_val);

                uint8_t new_state = 0b0000;

                if (left_bumper_val > HIGH_THRESHOLD) {
                    new_state |= (DEPRESSED << 4);
                } else if (left_bumper_val < LOW_THRESHOLD) {
                    new_state |= (IDLE << 4);
                }

                if (right_bumper_val > HIGH_THRESHOLD) {
                    new_state |= (DEPRESSED);
                } else if (right_bumper_val < LOW_THRESHOLD) {
                    new_state |= (IDLE);
                }

                // Small state machine transitions between states every 2 ms at the timeout
                if (old_state != new_state) {
                    printf("[Bumper Status: %04x]\n", new_state);
                    old_state = new_state;
                    if (old_state) {
                        ReturnEvent.EventType = BUMPER_TRIPPED;
                        ReturnEvent.EventParam = old_state;
                    }
                    PostTopHSM(ReturnEvent);
                    ES_Timer_InitTimer(BUMPER_TIMER, 500);
                }
                break;
                //#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
                //    default:
                //        printf("\r\nEvent: %s\tParam: 0x%04X",
                //                EventNames[ThisEvent.EventType], ThisEvent.EventParam);
                //        break;
                //#endif
            }
    }

    return ReturnEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/



