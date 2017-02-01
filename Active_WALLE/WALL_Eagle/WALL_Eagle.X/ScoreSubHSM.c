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
#include "ScoreSubHSM.h"
#include "TapeFollowSM.h"
#include "motors.h"
#include "DepositBallsSM.h"
#include "ExclusionRadar.h"
#include "BumpResService.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define MOVE_BACK_TIME 800
#define BACK_UP_TIME 600
#define DEPOSIT_TIME 200
#define TURN_AROUND_TIME 1950
#define DETACH_TIME 2000
#define MINI_SWEEP_TIME 400


#define FAST_SPEED 500
#define SLOW_SPEED 300
#define SEARCH_SPEED 250

typedef enum {
    InitPSubState,
    MoveCenterState,
    RadarState,
    MiniRadarState0,
    MiniRadarState1,
    ZoneInState,
    ReFind,
    BackUpState,
    TurnAroundState,
    MoveForwardState,
    DropBallState,
    DropBallAgainState,
    DetachState,
    ExRadarState,
    BumpResState,
} TemplateSubHSMState_t;

static const char *StateNames[] = {
	"InitPSubState",
	"MoveCenterState",
	"RadarState",
	"MiniRadarState0",
	"MiniRadarState1",
	"ZoneInState",
	"ReFind",
	"BackUpState",
	"TurnAroundState",
	"MoveForwardState",
	"DropBallState",
	"DropBallAgainState",
	"DetachState",
	"ExRadarState",
	"BumpResState",
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

uint8_t numtapes = 0;
int sweep_time = MINI_SWEEP_TIME;
uint8_t last_bump_in = 0;


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
uint8_t InitScoreSubHSM(void) {
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunScoreSubHSM(INIT_EVENT);
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
ES_Event RunScoreSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    TemplateSubHSMState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (ThisEvent.EventType) {
        case TAPE_SENSOR_TRIPPED:
            numtapes = NumTapesTripped(ThisEvent.EventParam);
            break;
        case BUMPER_TRIPPED:
            last_bump_in = ThisEvent.EventParam;
            nextState = BumpResState;
            makeTransition = TRUE;
            ThisEvent.EventType = ES_NO_EVENT;
    }

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                InitDepositBallsSM();
                InitExclusionRadar();
                nextState = MoveCenterState;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case MoveCenterState: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(SCORE_TIMER, MOVE_BACK_TIME);
                    break;
                case ES_TIMEOUT:
                    nextState = RadarState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    BotMoveForward(FAST_SPEED);
                    break;
            }
            break;

        case RadarState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotTankTurnLeft(SEARCH_SPEED);
                    break;
                case BEACON_FOUND:
                    nextState = ZoneInState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    BotTankTurnLeft(SEARCH_SPEED);
                    break;
            }
            break;

        case MiniRadarState0:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotTankTurnLeft(SEARCH_SPEED);
                    ES_Timer_InitTimer(SCORE_TIMER, sweep_time);
                    break;
                case BEACON_FOUND:
                    nextState = ZoneInState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == SCORE_TIMER) {
                        nextState = MiniRadarState1;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    BotTankTurnLeft(SEARCH_SPEED);
                    break;
            }
            break;

        case MiniRadarState1:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotTankTurnRight(SEARCH_SPEED);
                    sweep_time *= 2;
                    ES_Timer_InitTimer(SCORE_TIMER, sweep_time);
                    break;
                case BEACON_FOUND:
                    nextState = ZoneInState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == SCORE_TIMER) {
                        nextState = MiniRadarState0;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    BotTankTurnRight(SEARCH_SPEED);
                    break;
            }
            break;

        case ZoneInState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotMoveForward(FAST_SPEED);
                    break;
                case BEACON_LOST:
                    nextState = MiniRadarState0;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    sweep_time = MINI_SWEEP_TIME;
                    break;
                case TAPE_SENSOR_TRIPPED:
                    if (ThisEvent.EventParam & 0x1000 && ThisEvent.EventParam & 0x0001) {
                        nextState = BackUpState;
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

        case BackUpState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(SCORE_TIMER, BACK_UP_TIME);
                    BotMoveBackward(SLOW_SPEED);
                    break;
                case ES_TIMEOUT:
                    nextState = TurnAroundState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                default:
                    BotMoveBackward(FAST_SPEED);
                    break;
            }
            break;

        case TurnAroundState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(SCORE_TIMER, TURN_AROUND_TIME);
                    break;
                case ES_TIMEOUT:
                    nextState = MoveForwardState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                default:
                    BotTankTurnLeft(FAST_SPEED);
                    break;
            }
            break;

        case MoveForwardState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(SCORE_TIMER, DEPOSIT_TIME);
                    break;
                case ES_TIMEOUT:
                    nextState = DropBallState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                default:
                    BotMoveBackward(FAST_SPEED);
                    break;
            }
            break;

        case DropBallState:
            ThisEvent = RunDepositBallsSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotHalt();
                    InitDepositBallsSM();
                    break;
                case BALL_DROPPED:
                    nextState = DropBallAgainState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case DropBallAgainState:
            ThisEvent = RunDepositBallsSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    BotHalt();
                    InitDepositBallsSM();
                    break;
                case BALL_DROPPED:
                    nextState = DetachState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case DetachState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(SCORE_TIMER, DETACH_TIME);
                    break;
                case ES_TIMEOUT:
                    nextState = ExRadarState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    BotMoveForward(SLOW_SPEED);
                    break;
            }
            break;

        case ExRadarState:
            ThisEvent = RunExclusionRadar(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    InitExclusionRadar();
                    break;
                case BEACON_FOUND:
                    nextState = ZoneInState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case BumpResState: // in the first state, replace this with correct names
            ThisEvent = RunBumpResService(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    InitBumpResService(last_bump_in);
                    break;
                case BUMP_RESOLVED:
                    nextState = MoveCenterState;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunScoreSubHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunScoreSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
uint8_t NumTapesTripped(uint16_t param) {
    uint8_t num = 0;
    if (param & (0x0001)) num++;
    if (param & (0x0010)) num++;
    if (param & (0x0100)) num++;
    if (param & (0x1000)) num++;
    return num;
}
