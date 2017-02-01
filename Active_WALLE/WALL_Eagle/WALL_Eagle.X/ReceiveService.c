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
#include "timers.h"
#include "ReceiveService.h"
#include "stdio.h"
#include "BumperService.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define MOVE_BACK_TIME 1200
#define ROTATION_TIME 800
#define ATTEMPT_TIME 1200
#define RAM_TIME 2000
#define SLIGHT_TURN_TIME 300
#define REV_TIME 150

#define FAST_SPEED 600
#define SLOW_SPEED 300
#define LUDACROUS_SPEED 1000

#define LEFT_HIT 0x10
#define RIGHT_HIT 0x01

typedef enum {
    InitPSubState,
    HitThatShit,
    BackUpState,
    Turn45State,
    DriveInState,
    RealignState,
    RamState1,
    RevState2,
    AngleState2,
    RamState2,
    RevState3,
    AngleState3,
    RamState3,
} BumperResState_t;

static const char *StateNames[] = {
	"InitPSubState",
	"HitThatShit",
	"BackUpState",
	"Turn45State",
	"DriveInState",
	"RealignState",
	"RamState1",
	"RevState2",
	"AngleState2",
	"RamState2",
	"RevState3",
	"AngleState3",
	"RamState3",
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

static BumperResState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;

uint8_t last_bump_received = 0;



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
uint8_t InitReceiveService(uint8_t last_bump) {
    ES_Event returnEvent;
    //TIMERS_Init();
    CurrentState = InitPSubState;
    returnEvent = RunReceiveService(INIT_EVENT);
    //ES_Timer_InitTimer(RECEIVE_TIMEOUT_TIMER, 10000);
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
ES_Event RunReceiveService(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    BumperResState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = HitThatShit;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case HitThatShit:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, 1500);
                    break;
                case BUMPER_TRIPPED:
                    last_bump_received = ThisEvent.EventParam;
                    nextState = BackUpState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == RECEIVE_TIMER) {
                        nextState = InitPSubState;
                        makeTransition = TRUE;
                        ThisEvent.EventType = AMMO_PLUNGER_LOST;
                    }
                case ES_NO_EVENT:
                default:
                    BotMoveForward(SLOW_SPEED);
                    break;
            }
            break;

        case BackUpState: // in the first state, replace this with correct names
            //printf("\n<<1>>\n");
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, MOVE_BACK_TIME);
                    //last_bump = old_state;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == RECEIVE_TIMER) {
                        nextState = Turn45State;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    BotMoveBackward(FAST_SPEED);
                    break;
            }
            break;
        case Turn45State: // in the first state, replace this with correct names
            //printf("\n<<2>>\n");
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, ROTATION_TIME);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == RECEIVE_TIMER) {
                        nextState = DriveInState;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    if (last_bump_received == LEFT_HIT) BotTankTurnRight(SLOW_SPEED);
                    if (last_bump_received == RIGHT_HIT) BotTankTurnLeft(SLOW_SPEED);
                    break;
            }
            break;
        case DriveInState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, ATTEMPT_TIME);
                    break;
                case TRACK_WIRE_FOUND:
                    if (ThisEvent.EventParam & 0x10) {
                        nextState = RealignState;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    BotMoveForward(SLOW_SPEED);
                    break;
            }
            break;
        case RealignState: // in the first state, replace this with correct names
            printf("\n<<3>>\n");
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, RAM_TIME);
                    if (last_bump_received == LEFT_HIT) {
                        BotTankTurnRight(SLOW_SPEED);
                    }
                    if (last_bump_received == RIGHT_HIT) {
                        BotTankTurnLeft(SLOW_SPEED);
                    }
                    //BotMoveBackward(LUDACROUS_SPEED);
                    break;
                case TRACK_WIRE_FOUND:
                    if (ThisEvent.EventParam & 0x01) {
                        nextState = RamState1;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case RamState1: // in the first state, replace this with correct names
            //printf("\n<<1>>\n");
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, RAM_TIME);
                    //last_bump = old_state;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == RECEIVE_TIMER) {
                        nextState = RevState2;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    BotMoveBackward(FAST_SPEED);
                    break;
            }
            break;
        case RevState2: // in the first state, replace this with correct names
            //printf("\n<<1>>\n");
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, REV_TIME);
                    //last_bump = old_state;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == RECEIVE_TIMER) {
                        nextState = RamState2;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    BotMoveForward(FAST_SPEED);
                    break;
            }
            break;
        case RamState2: // in the first state, replace this with correct names
            //printf("\n<<1>>\n");
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, RAM_TIME);
                    //last_bump = old_state;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == RECEIVE_TIMER) {
                        nextState = RevState3;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    BotRBankTurnLeft(FAST_SPEED, 3);
                    break;
            }
            break;
        case RevState3: // in the first state, replace this with correct names
            //printf("\n<<1>>\n");
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, REV_TIME);
                    //last_bump = old_state;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == RECEIVE_TIMER) {
                        nextState = RamState3;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    BotBankTurnLeft(FAST_SPEED, 3);
                    break;
            }
            break;
        case RamState3: // in the first state, replace this with correct names
            //printf("\n<<1>>\n");
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(RECEIVE_TIMER, RAM_TIME);
                    //last_bump = old_state;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == RECEIVE_TIMER) {
                        nextState = InitPSubState;
                        makeTransition = TRUE;
                        ThisEvent.EventType = BALLS_LOADED;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    BotRBankTurnRight(FAST_SPEED, 3);
                    break;
            }
            break;
        default: // all unhandled states fall into here
            break;
    } // end switch on Current State
    //
    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunReceiveService(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunReceiveService(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

