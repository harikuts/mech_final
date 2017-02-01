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
#include "DepositBallsSM.h"
#include "RC_Servo.h"
#include "PinsMaster.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

#define GATE_TIME 800
#define BUFFER_TIME 200


#define SERVO_0_OPEN RC_SetPulseTime(SERVO_0_OUT_PIN,1000)
#define SERVO_0_CLOSE RC_SetPulseTime(SERVO_0_OUT_PIN,2000)
#define SERVO_1_OPEN RC_SetPulseTime(SERVO_1_OUT_PIN,1000)
#define SERVO_1_CLOSE RC_SetPulseTime(SERVO_1_OUT_PIN,2000)

typedef enum {
    InitPSubState,
    OpenState0,
    CloseState0,
    testState2,
    OpenState1,
    CloseState1,
    testState,
} DepositBallsSMState_t;

static const char *StateNames[] = {
	"InitPSubState",
	"OpenState0",
	"CloseState0",
	"testState2",
	"OpenState1",
	"CloseState1",
	"testState",
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

static DepositBallsSMState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;


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
uint8_t InitDepositBallsSM(void) {
    ES_Event returnEvent;

    TIMERS_Init();

    CurrentState = InitPSubState;
    returnEvent = RunDepositBallsSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

uint8_t PostDepositBallsSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
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
ES_Event RunDepositBallsSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    DepositBallsSMState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = OpenState0;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case OpenState0: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    SERVO_0_OPEN;
                    ES_Timer_InitTimer(DEPOSIT_TIMER, GATE_TIME);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DEPOSIT_TIMER) {
                        nextState = CloseState0;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default:
                    break;
            }
            break;

        case CloseState0: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    SERVO_0_CLOSE;
                    ES_Timer_InitTimer(DEPOSIT_TIMER, GATE_TIME);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DEPOSIT_TIMER) {
                        nextState = testState2;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default:
                    break;
            }
            break;

        case testState2:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    SERVO_0_OPEN;
                    ES_Timer_InitTimer(DEPOSIT_TIMER, BUFFER_TIME);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DEPOSIT_TIMER) {
                        nextState = OpenState1;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default:
                    break;
            }
            break;

        case OpenState1: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    SERVO_1_OPEN;
                    ES_Timer_InitTimer(DEPOSIT_TIMER, GATE_TIME);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DEPOSIT_TIMER) {
                        nextState = CloseState1;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default:
                    break;
            }
            break;

        case CloseState1: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    SERVO_1_CLOSE;
                    ES_Timer_InitTimer(DEPOSIT_TIMER, GATE_TIME);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DEPOSIT_TIMER) {
                        nextState = testState;
                        makeTransition = TRUE;
                        ThisEvent.EventType = BALL_DROPPED;
                    }
                    break;
                case ES_NO_EVENT:
                default:
                    break;
            }
            break;

        case testState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    SERVO_1_OPEN;
                    ES_Timer_InitTimer(DEPOSIT_TIMER, BUFFER_TIME);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DEPOSIT_TIMER) {
                        nextState = InitPSubState;
                        makeTransition = TRUE;
                        ThisEvent.EventType = BALL_DROPPED;
                    }
                    break;
                case ES_NO_EVENT:
                default:
                    break;
            }
            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunDepositBallsSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunDepositBallsSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

