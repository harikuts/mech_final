/*
 * File: TemplateSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 *
 * Template file to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is for a substate machine. Make sure it has a unique name
 *
 * This is provided as an example and a good place to start.
 *
 * History
 * When           Who     What/Why
 * -------------- ---     --------
 * 09/13/13 15:17 ghe      added tattletail functionality and recursive calls
 * 01/15/12 11:12 jec      revisions for Gen2 framework
 * 11/07/11 11:26 jec      made the queue static
 * 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
 * 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "TopHSM.h"
#include "TapeFollowSM.h"
#include "motors.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define FAST_SPEED 400
#define SLOW_SPEED 300

#define STRAIGHT_TIME 2000
#define LOST_TIMEOUT 3500
#define TURN_TIME 600
#define ESCAPE_TIMEOUT 4000

// Tape Sensor Param Masks
#define FR_MASK 0x0001
#define MR_MASK 0x0010
#define ML_MASK 0x0100
#define FL_MASK 0x1000

typedef enum {
    InitPSubState,
    MoveForward,
            ShiftDirection,
    FoundRightTape,
    RightRealign,
    RightTapeLost,
    FoundLeftTape,
    LeftRealign,
    LeftTapeLost,
} TapeFollowSMState_t;

static const char *StateNames[] = {
	"InitPSubState",
	"MoveForward",
	"ShiftDirection",
	"FoundRightTape",
	"RightRealign",
	"RightTapeLost",
	"FoundLeftTape",
	"LeftRealign",
	"LeftTapeLost",
};



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static TapeFollowSMState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;

//Status indicators
uint8_t FR_tripped = FALSE;
uint8_t MR_tripped = FALSE;
uint8_t ML_tripped = FALSE;
uint8_t FL_tripped = FALSE;

//uint8_t last_tw = 0;
uint8_t been_awhile = FALSE;



/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateSubHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitTapeFollowSM(void) {
    ES_Event returnEvent;

    ES_Timer_InitTimer(TAPE_FOLLOW_STARTUP_TIMER, 500);

    CurrentState = MoveForward;
    returnEvent = RunTapeFollowSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunTemplateSubHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the heirarchical state
 *        machine, as this is called any time a new event is passed to the event
 *        queue. This function will be called recursively to implement the correct
 *        order for a state transition to be: exit current state -> enter next state
 *        using the ES_EXIT and ES_ENTRY events.
 * @note Remember to rename to something appropriate.
 *       The lower level state machines are run first, to see if the event is dealt
 *       with there rather than at the current level. ES_EXIT and ES_ENTRY events are
 *       not consumed as these need to pass pack to the higher level state machine.
 * @author J. Edward Carryer, 2011.10.23 19:25
 * @author Gabriel H Elkaim, 2011.10.23 19:25 */
ES_Event RunTapeFollowSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    TapeFollowSMState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    // Assemble True/False declarations for each tape sensor
    if (ThisEvent.EventType == TAPE_SENSOR_TRIPPED) {
        if (ThisEvent.EventParam & FR_MASK) {
            FR_tripped = TRUE;
        } else {
            FR_tripped = FALSE;
        }
        if (ThisEvent.EventParam & MR_MASK) {
            MR_tripped = TRUE;
        } else {
            MR_tripped = FALSE;
        }
        if (ThisEvent.EventParam & ML_MASK) {
            ML_tripped = TRUE;
        } else {
            ML_tripped = FALSE;
        }
        if (ThisEvent.EventParam & FL_MASK) {
            FL_tripped = TRUE;
        } else {
            FL_tripped = FALSE;
        }
    }
    



    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_TIMEOUT && ThisEvent.EventParam == TAPE_FOLLOW_STARTUP_TIMER)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = MoveForward;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case MoveForward:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotMoveForward(FAST_SPEED);
                    ES_Timer_InitTimer(TAPE_FOLLOW_TIMER, LOST_TIMEOUT);
                    break;
                case TAPE_SENSOR_TRIPPED:
                    ThisEvent.EventType = ES_NO_EVENT;
                    if ((FR_tripped | MR_tripped) && !ML_tripped && !FL_tripped) {
                        nextState = FoundRightTape;
                        makeTransition = TRUE;
                        //ThisEvent.EventType = ES_NO_EVENT;
                    } else if ((FL_tripped | ML_tripped) && !MR_tripped && !FR_tripped) {
                        nextState = FoundLeftTape;
                        makeTransition = TRUE;
                        //ThisEvent.EventType = ES_NO_EVENT;
                    } else if (!FL_tripped && !ML_tripped && !MR_tripped && !FR_tripped) {
                        ; // do nothing
                    } else {
                        //BotRPivotLeft(SLOW_SPEED);
                        ;
                    }
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TAPE_FOLLOW_TIMER){
                        nextState = ShiftDirection;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
            
        case ShiftDirection:
            BotTankTurnLeft(300);
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(TAPE_FOLLOW_TIMER, TURN_TIME);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TAPE_FOLLOW_TIMER) {
                        nextState = MoveForward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
            
        case FoundRightTape:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotMoveForward(SLOW_SPEED);
                    ES_Timer_InitTimer(TAPE_FOLLOW_TIMER, STRAIGHT_TIME);
                    been_awhile = FALSE;
                    break;
                case TAPE_SENSOR_TRIPPED:
                    ThisEvent.EventType = ES_NO_EVENT;
                    if (MR_tripped || !FR_tripped) {
                        nextState = RightRealign;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    if (!FR_tripped) {
                        nextState = RightTapeLost;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    if (FL_tripped) {
                        nextState = MoveForward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = BUMPER_TRIPPED;
                        ThisEvent.EventParam = 0x01;
                    }
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TAPE_FOLLOW_TIMER) {
                        been_awhile = TRUE;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case RightRealign:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    if (been_awhile) BotRRPivotLeft(SLOW_SPEED);
                    else BotRPivotLeft(SLOW_SPEED);
                    break;
                case TAPE_SENSOR_TRIPPED:
                    ThisEvent.EventType = ES_NO_EVENT;
                    if (!MR_tripped && FR_tripped) {
                        nextState = FoundRightTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    if (!MR_tripped && !FR_tripped) {
                        nextState = MoveForward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TAPE_FOLLOW_TIMER) {
                        been_awhile = TRUE;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case RightTapeLost:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotRRPivotRight(SLOW_SPEED);
                    ES_Timer_InitTimer(TAPE_FOLLOW_TIMER, ESCAPE_TIMEOUT);
                    break;
                case TAPE_SENSOR_TRIPPED:
                    ThisEvent.EventType = ES_NO_EVENT;
                    if (FR_tripped) {
                        nextState = FoundRightTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TAPE_FOLLOW_TIMER){
                        nextState = MoveForward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case FoundLeftTape:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotMoveForward(SLOW_SPEED);
                    ES_Timer_InitTimer(TAPE_FOLLOW_TIMER, STRAIGHT_TIME);
                    been_awhile = FALSE;
                    break;
                case TAPE_SENSOR_TRIPPED:
                    ThisEvent.EventType = ES_NO_EVENT;
                    if (ML_tripped || !FL_tripped) {
                        nextState = LeftRealign;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    if (!FL_tripped) {
                        nextState = LeftTapeLost;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    if (FR_tripped) {
                        nextState = MoveForward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = BUMPER_TRIPPED;
                        ThisEvent.EventParam = 0x10;
                    }
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TAPE_FOLLOW_TIMER) {
                        been_awhile = TRUE;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case LeftRealign:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    if (been_awhile) BotRRPivotRight(SLOW_SPEED);
                    else BotRPivotRight(SLOW_SPEED);
                    break;
                case TAPE_SENSOR_TRIPPED:
                    ThisEvent.EventType = ES_NO_EVENT;
                    if (!ML_tripped && FL_tripped) {
                        nextState = FoundLeftTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    if (!ML_tripped && !FL_tripped) {
                        nextState = MoveForward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case LeftTapeLost:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotRRPivotLeft(SLOW_SPEED);
                    ES_Timer_InitTimer(TAPE_FOLLOW_TIMER, ESCAPE_TIMEOUT);
                    break;
                case TAPE_SENSOR_TRIPPED:
                    ThisEvent.EventType = ES_NO_EVENT;
                    if (FL_tripped) {
                        nextState = FoundLeftTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TAPE_FOLLOW_TIMER){
                        nextState = MoveForward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunTapeFollowSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunTapeFollowSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

