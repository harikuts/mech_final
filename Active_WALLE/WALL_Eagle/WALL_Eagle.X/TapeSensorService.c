
/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "BOARD.h"
#include "AD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TapeSensorService.h"
#include "string.h"
#include "ES_Timers.h"
#include "IO_Ports.h"
#include <stdio.h>
#include "PinsMaster.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define ARCHIVE_SIZE 10

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */
void ArchiveValue(uint16_t value);
uint16_t FindArchiveMean(void);
uint16_t FindMovingMean(void);


/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
/* You will need MyPriority and maybe a state variable; you may need others
 * as well. */

static uint8_t MyPriority;

sampleState currentState;

unsigned int sample1;
unsigned int sample2;

uint16_t tape_data = 0;
uint16_t new_tape_data = 0;

uint16_t tape_archive[ARCHIVE_SIZE] = {0};
int archive_index = 0;

char output[32];

tape_sensor FL, ML, MR, FR;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t InitTapeSensorService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;

    //AD_Init();
    //AD_AddPins(FAR_LEFT_TAPE_SENSOR_PIN | MID_LEFT_TAPE_SENSOR_PIN | MID_RIGHT_TAPE_SENSOR_PIN | FAR_RIGHT_TAPE_SENSOR_PIN);
    IO_PortsSetPortOutputs(FLASH_PIN);
    InitMotors();

    // Initialize state machine
    currentState = SAMPLE_FLASH_ON;
    FL.status = WHITE;

    // Start the timer and let it run expiring every 2 ms
    ES_Timer_InitTimer(TAPE_SENSOR_TIMER, 500);

    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

uint8_t PostTapeSensorService(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

ES_Event RunTapeSensorService(ES_Event ThisEvent) {
    ES_Event ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors

    /********************************************
     in here you write your service code
     *******************************************/
    static ES_EventTyp_t lastEvent = WHITE_TAPE_DETECTED;
    ES_EventTyp_t curEvent;
    sampleState nextState;
    uint8_t makeTransition = FALSE;
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
            ES_Timer_InitTimer(TAPE_SENSOR_TIMER, 2);
            // Small state machine transitions between states every 2 ms at the timeout
            switch (currentState) {
                case SAMPLE_FLASH_ON: // Turn LEDs on
                    //printf("1");
                    FLASH_ON;
                    if (AD_IsNewDataReady()) {
                        nextState = SAMPLE_TAKE_SAMPLE_1;
                        makeTransition = TRUE;
                    }
                    break;
                case SAMPLE_TAKE_SAMPLE_1: // Take first sample
                    //printf("2");
                    TakeFirstSample();
                    nextState = SAMPLE_FLASH_OFF;
                    makeTransition = TRUE;
                    break;
                case SAMPLE_FLASH_OFF: // Turn LEDs off
                    //printf("3");
                    FLASH_OFF;
                    if (AD_IsNewDataReady()) {
                        nextState = SAMPLE_TAKE_SAMPLE_2;
                        makeTransition = TRUE;
                    }
                    break;
                case SAMPLE_TAKE_SAMPLE_2: // Take second sample and perform calculations
                    //printf("4");
                    TakeSecondSample();
                    AssembleReadings();
                    FindMovingMean();
                    //printf("\n[Tape Status:\t%04x]\n[Tape Values:\t%d\t%d\t%d\t%d]\n\n", new_tape_data, FL.diff, ML.diff, MR.diff, FR.diff);
                    //printf("[New: %d Old: %d]\n", new_tape_data, tape_data);
                    if (new_tape_data != tape_data) {
                        
                        tape_data = new_tape_data;
                        ReturnEvent.EventType = TAPE_SENSOR_TRIPPED;
                        ReturnEvent.EventParam = tape_data;
                        PostTopHSM(ReturnEvent);
                        ES_Timer_InitTimer(TAPE_SENSOR_TIMER, 20);
                    }
                    nextState = SAMPLE_FLASH_ON;
                    makeTransition = TRUE;
                    break;
            }
            if (makeTransition) {
                currentState = nextState;
                makeTransition = FALSE;
            }
            break;
            //#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
            //    default:
            //        printf("\r\nEvent: %s\tParam: 0x%04X",
            //                EventNames[ThisEvent.EventType], ThisEvent.EventParam);
            //        break;
            //#endif
    }

    return ReturnEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/

uint8_t dummy_func(void) {
    return 0;
    // Do Nothing
}

void TakeFirstSample(void) {
    FL.sample1 = AD_ReadADPin(FAR_LEFT_TAPE_SENSOR_PIN);
    ML.sample1 = AD_ReadADPin(MID_LEFT_TAPE_SENSOR_PIN);
    MR.sample1 = AD_ReadADPin(MID_RIGHT_TAPE_SENSOR_PIN);
    FR.sample1 = AD_ReadADPin(FAR_RIGHT_TAPE_SENSOR_PIN);
}

void TakeSecondSample(void) {
    FL.sample2 = AD_ReadADPin(FAR_LEFT_TAPE_SENSOR_PIN);
    ML.sample2 = AD_ReadADPin(MID_LEFT_TAPE_SENSOR_PIN);
    MR.sample2 = AD_ReadADPin(MID_RIGHT_TAPE_SENSOR_PIN);
    FR.sample2 = AD_ReadADPin(FAR_RIGHT_TAPE_SENSOR_PIN);
}

void UpdateStatus() {
    FL.diff = FL.sample2 - FL.sample1;
    //printf("|%d|\n", FL.diff);
    if (FL.diff > WHITE_TAPE_THRESHOLD) {
        FL.new_status = WHITE;
    }
    if (FL.diff < BLACK_TAPE_THRESHOLD) {
        FL.new_status = BLACK;
    }
    if (FL.new_status != FL.status) {
        FL.status = FL.new_status;
        //printf("%d", FL.status);
    }
    // ML
    ML.diff = ML.sample2 - ML.sample1;
    if (ML.diff > WHITE_TAPE_THRESHOLD) {
        ML.new_status = WHITE;
    }
    if (ML.diff < BLACK_TAPE_THRESHOLD) {
        ML.new_status = BLACK;
    }
    if (ML.new_status != ML.status) {
        ML.status = ML.new_status;
        //printf("%d", ML.status);
    }
    // MR
    MR.diff = MR.sample2 - MR.sample1;
    if (MR.diff > WHITE_TAPE_THRESHOLD) {
        MR.new_status = WHITE;
    }
    if (MR.diff < BLACK_TAPE_THRESHOLD) {
        MR.new_status = BLACK;
    }
    if (MR.new_status != MR.status) {
        MR.status = MR.new_status;
        //printf("%d", MR.status);
    }
    // FR
    FR.diff = FR.sample2 - FR.sample1;
    //printf("%d\n", FR.diff);
    if (FR.diff > WHITE_TAPE_THRESHOLD) {
        FR.new_status = WHITE;
    }
    if (FR.diff < BLACK_TAPE_THRESHOLD) {
        FR.new_status = BLACK;
    }
    if (FR.new_status != FR.status) {
        FR.status = FR.new_status;
        //printf("%d", FR.status);
    }
}

void AssembleReadings(void) {
    UpdateStatus();
    new_tape_data = 0;
    if (FL.status == BLACK) new_tape_data |= (1 << 12);
    if (ML.status == BLACK) new_tape_data |= (1 << 8);
    if (MR.status == BLACK) new_tape_data |= (1 << 4);
    if (FR.status == BLACK) new_tape_data |= 1;
}

void ArchiveValue(uint16_t value) {
    tape_archive[archive_index++] = value;
    if (archive_index >= ARCHIVE_SIZE) archive_index = 0;
}

uint16_t FindArchiveMean(void) {
    int points[8] = {0};
    int i, j;
    for (i = 0; i < ARCHIVE_SIZE; i++) {
        for (j = 0; j <= i; j++) {
            if (tape_archive[i] == tape_archive[j]) {
                points[j]++;
                continue;
            }
        }
    }
    int max_ind = 0;
    for (i = 0; i < ARCHIVE_SIZE; i++) {
        if (points[i] > points[max_ind]) {
            max_ind = i;
        }
    }
    return tape_archive[max_ind];
}

uint16_t FindMovingMean(void){
    ArchiveValue(new_tape_data);
    return FindArchiveMean();
}

