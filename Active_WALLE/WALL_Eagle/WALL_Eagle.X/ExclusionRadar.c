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
#include "ExclusionRadar.h"
#include "motors.h"
#include "TapeFollowSM.h"
#include "BumpResService.h"
#include "DepositBallsSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define FAST_SPEED 600
#define SLOW_SPEED 300
#define SLOW_20_DEG 360
#define SLOW_40_DEG 720
#define SLOW_90_DEG 1620
#define SLOW_120_DEG 2160
#define SHORT_TIME 1000
#define SCAN_TIME 5000

typedef enum {
    InitPSubState,
    SetState,
    BackingState,
    OneTwentyDegreeTurning,
    MovingForward,
    TapeFollowing,
    BumpResolving,
    NinetyDegreeSearching,
    NinetyDegreeReturning,
    Finding,
    AdjustingLeft,
    AdjustingRight,
    Dropping,

} TemplateSubHSMState_t;

static const char *StateNames[] = {
	"InitPSubState",
	"SetState",
	"BackingState",
	"OneTwentyDegreeTurning",
	"MovingForward",
	"TapeFollowing",
	"BumpResolving",
	"NinetyDegreeSearching",
	"NinetyDegreeReturning",
	"Finding",
	"AdjustingLeft",
	"AdjustingRight",
	"Dropping",
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

static TemplateSubHSMState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;

uint8_t previous_bump = 0;
uint8_t numbertapes = 0;


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
uint8_t InitExclusionRadar(void) {
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunExclusionRadar(INIT_EVENT);
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
ES_Event RunExclusionRadar(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    TemplateSubHSMState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    // EVENT UPDATERS
    switch (ThisEvent.EventType) {
        case BUMPER_TRIPPED:
            previous_bump = ThisEvent.EventParam;
            break;
        case TAPE_SENSOR_TRIPPED:
            numbertapes = NumTapesTripped(ThisEvent.EventParam);
            break;
    }

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                InitTapeFollowSM();
                InitBumpResService(previous_bump);
                nextState = BackingState;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case BackingState: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(EX_RADAR_TIMER, SHORT_TIME);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == EX_RADAR_TIMER) {
                        nextState = OneTwentyDegreeTurning;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    BotMoveForward(FAST_SPEED);
                    break;
            }
            break;

        case OneTwentyDegreeTurning: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(EX_RADAR_TIMER, SLOW_120_DEG);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == EX_RADAR_TIMER) {
                        InitTapeFollowSM();
                        nextState = TapeFollowing;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
//                case BEACON_FOUND:
//                    nextState = Finding;
//                    makeTransition = TRUE;
//                    ThisEvent.EventType = ES_NO_EVENT;
//                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    BotTankTurnLeft(SLOW_SPEED);
                    break;
            }
            break;

        case TapeFollowing: // in the first state, replace this with correct names
            ThisEvent = RunTapeFollowSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(EX_RADAR_TIMER, SCAN_TIME);
                    //InitTapeFollowSM();
                    BotMoveForward(300);
                    break;
                case BUMPER_TRIPPED:
                    nextState = BumpResolving;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == EX_RADAR_TIMER) {
                        nextState = NinetyDegreeSearching;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case BumpResolving: // in the first state, replace this with correct names
            ThisEvent = RunBumpResService(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    InitBumpResService(previous_bump);
                    break;
                case BUMP_RESOLVED:
                    nextState = TapeFollowing;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }

        case NinetyDegreeSearching: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(EX_RADAR_TIMER, SLOW_90_DEG);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == EX_RADAR_TIMER) {
                        nextState = NinetyDegreeReturning;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
//                case BEACON_FOUND:
//                    nextState = Finding;
//                    makeTransition = TRUE;
//                    ThisEvent.EventType = ES_NO_EVENT;
//                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    BotTankTurnLeft(SLOW_SPEED);
                    break;
            }
            break;

        case NinetyDegreeReturning: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(EX_RADAR_TIMER, SLOW_90_DEG);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == EX_RADAR_TIMER) {
                        nextState = TapeFollowing;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    BotTankTurnRight(SLOW_SPEED);
                    break;
            }
            break;

            //        case Finding: // in the first state, replace this with correct names
            //            switch (ThisEvent.EventType) {
            //                case BEACON_LOST:
            //                    nextState = AdjustingLeft;
            //                    makeTransition = TRUE;
            //                    ThisEvent.EventType = ES_NO_EVENT;
            //                    break;
            //                case TAPE_SENSOR_TRIPPED:
            //                    if (ThisEvent.EventParam == 0x1001) {
            //                        nextState = Dropping;
            //                        makeTransition = TRUE;
            //                        ThisEvent.EventType = ES_NO_EVENT;
            //                    } else {
            //                        nextState = TapeFollowing;
            //                        makeTransition = TRUE;
            //                        ThisEvent.EventType = ES_NO_EVENT;
            //                    }
            //                case ES_NO_EVENT:
            //                default: // all unhandled events pass the event back up to the next level
            //                    BotMoveForward(SLOW_SPEED);
            //                    break;
            //            }
            //            break;
            //
            //        case AdjustingLeft:
            //            switch (ThisEvent.EventType) {
            //                case ES_ENTRY:
            //                    ES_Timer_InitTimer(EX_RADAR_TIMER, SLOW_20_DEG);
            //                    break;
            //                case BEACON_FOUND:
            //                    nextState = Finding;
            //                    makeTransition = TRUE;
            //                    ThisEvent.EventType = ES_NO_EVENT;
            //                    break;
            //                case ES_TIMEOUT:
            //                    nextState = AdjustingRight;
            //                    makeTransition = TRUE;
            //                    ThisEvent.EventType = ES_NO_EVENT;
            //                    break;
            //                case ES_NO_EVENT:
            //                default: // all unhandled events pass the event back up to the next level
            //                    BotTankTurnLeft(SLOW_SPEED);
            //                    break;
            //            }
            //            break;
            //
            //        case AdjustingRight:
            //            switch (ThisEvent.EventType) {
            //                case ES_ENTRY:
            //                    ES_Timer_InitTimer(EX_RADAR_TIMER, SLOW_40_DEG);
            //                    break;
            //                case ES_TIMEOUT:
            //                    nextState = OneTwentyDegreeTurning;
            //                    makeTransition = TRUE;
            //                    ThisEvent.EventType = ES_NO_EVENT;
            //                    break;
            //                case BEACON_FOUND:
            //                    nextState = Finding;
            //                    makeTransition = TRUE;
            //                    ThisEvent.EventType = ES_NO_EVENT;
            //                    break;
            //                case ES_NO_EVENT:
            //                default: // all unhandled events pass the event back up to the next level
            //                    BotTankTurnRight(SLOW_SPEED);
            //                    break;
            //            }
            //            break;
            //
            //        case Dropping: // in the first state, replace this with correct names
            //             ThisEvent = RunDepositBallsSM(ThisEvent);
            //            switch (ThisEvent.EventType) {
            //                case ES_ENTRY:
            //                    BotHalt();
            //                    InitDepositBallsSM();
            //                    break;
            //                case BALL_DROPPED:
            //                    nextState = OneTwentyDegreeTurning;
            //                    makeTransition = TRUE;
            //                    ThisEvent.EventType = ES_NO_EVENT;
            //                    break;
            //                case ES_NO_EVENT:
            //                default: // all unhandled events pass the event back up to the next level
            //                    break;
            //            }
            //            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunExclusionRadar(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunExclusionRadar(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

