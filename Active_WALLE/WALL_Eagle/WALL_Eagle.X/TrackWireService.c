
/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "BOARD.h"
#include "AD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TrackWireService.h"
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


#define TW_HIGH_THRESHOLD 900
#define TW_LOW_THRESHOLD 600

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/

/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */



/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
/* You will need MyPriority and maybe a state variable; you may need others
 * as well. */

static uint8_t MyPriority;

static ES_EventTyp_t lastEvent = TRACK_WIRE_LOST;
static uint16_t tw_status = 0x00;

ES_EventTyp_t curEvent;

uint16_t trackwire0_signal; // read the battery voltage
uint16_t trackwire1_signal;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t InitTrackWireService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;


    //AD_AddPins(AD_ActivePins() | LEFT_BUMPER_PIN | RIGHT_BUMPER_PIN);

    // Start the timer and let it run expiring every 2 ms
    ES_Timer_InitTimer(TRACK_WIRE_TIMER, 500);

    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

uint8_t PostTrackWireService(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

ES_Event RunTrackWireService(ES_Event ThisEvent) {
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
            if (ThisEvent.EventParam == TRACK_WIRE_TIMER) {
                //printf("*");
                ES_Timer_InitTimer(TRACK_WIRE_TIMER, 5);
                if (AD_IsNewDataReady()) {
                    ES_Event thisEvent;
                    trackwire0_signal = AD_ReadADPin(TW_DETECTOR_0_PIN); // read the battery voltage
                    trackwire1_signal = AD_ReadADPin(TW_DETECTOR_1_PIN);
                    //printf("|%d - %d|\n", trackwire0_signal, trackwire1_signal);
                    uint8_t tw0 = FALSE;
                    uint8_t tw1 = FALSE;

                    // Determine state of track wires
                    if (trackwire0_signal > TW_HIGH_THRESHOLD) { // is battery connected?
                        tw0 = FALSE;
                    } else if (trackwire0_signal < TW_LOW_THRESHOLD) {
                        tw0 = TRUE;
                    }
                    if (trackwire1_signal > TW_HIGH_THRESHOLD) { // is battery connected?
                        tw1 = FALSE;
                    } else if (trackwire1_signal < TW_LOW_THRESHOLD) {
                        tw1 = TRUE;
                    }

                    // Assemble the readings
                    uint16_t new_tw_status = 0;
                    new_tw_status = tw0 | (tw1 << 4);
                    // Determine events
                    if (new_tw_status == 0) curEvent = TRACK_WIRE_LOST;
                    else curEvent = TRACK_WIRE_FOUND;
                    //printf("|%d - %d|\n", trackwire0_signal, trackwire1_signal);

                    if (tw_status != new_tw_status) { // check for change from last time
                        //printf("lmao");
                        tw_status = new_tw_status;
                        thisEvent.EventType = curEvent;
                        thisEvent.EventParam = tw_status;
                        lastEvent = curEvent; // update history
                        ES_Timer_InitTimer(TRACK_WIRE_TIMER, 20);
                        PostTopHSM(thisEvent);
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
    }

    return ReturnEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/



