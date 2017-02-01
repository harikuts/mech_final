/*
 * File: TemplateFSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 *
 * Template file to set up a Flat State Machine to work with the Events and Services
 * Frameword (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that this file
 * will need to be modified to fit your exact needs, and most of the names will have
 * to be changed to match your code.
 *
 * This is provided as an example and a good place to start.
 *
 *Generally you will just be modifying the statenames and the run function
 *However make sure you do a find and replace to convert every instance of
 *  "Template" to your current state machine's name
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

#include "TemplateFSM.h"
#include <BOARD.h>
//Uncomment these for the Roaches
//#include "roach.h"
//#include "RoachFrameworkEvents.h"
#include <stdio.h>


/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define DARK_THRESHOLD 900

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine.*/


/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
uint8_t escapeSeq = FALSE;
uint8_t hellaRight = FALSE;
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

typedef enum {
    InitPState,
//    KillState,
    FirstState,
    CheckState,
    LightState,
    DarkState,
    AboutFaceState,
    LeftFaceState,
    RightFaceState,
    ForwardState,

} TemplateFSMState_t;

static const char *StateNames[] = {
	"InitPState",
	"FirstState",
	"CheckState",
	"LightState",
	"DarkState",
	"AboutFaceState",
	"LeftFaceState",
	"RightFaceState",
	"ForwardState",
};


static TemplateFSMState_t CurrentState = InitPState; // <- change enum name to match ENUM
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateFSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitTemplateFSM(uint8_t Priority) {
    MyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = InitPState;
    // post the initial transition event
    if (ES_PostToService(MyPriority, INIT_EVENT) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function PostTemplateFSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostTemplateFSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function RunTemplateFSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the flat state machine,
 *        as this is called any time a new event is passed to the event queue. This
 *        function will be called recursively to implement the correct order for a
 *        state transition to be: exit current state -> enter next state using the
 *        ES_EXIT and ES_ENTRY events.
 * @note Remember to rename to something appropriate.
 *       Returns ES_NO_EVENT if the event have been "consumed."
 * @author J. Edward Carryer, 2011.10.23 19:25 */
ES_Event RunTemplateFSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    TemplateFSMState_t nextState; // <- need to change enum type here

    ES_Tattle(); // trace call stack


    switch (CurrentState) {
        case InitPState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = FirstState;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case FirstState: // in the first state, replace this with appropriate state

            switch (ThisEvent.EventType) {
                case LIGHT:
                    moveForward();
                    nextState = LightState;
                    makeTransition = TRUE;
                    break;
                case DARK:
                    stop();
                    nextState = DarkState;
                    makeTransition = TRUE;
                    break;
            }
            break;

        case CheckState:
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    inchForward();
                    if (Roach_LightLevel() > DARK_THRESHOLD) {
                        stop();
                        nextState = DarkState;
                    } else {
                        moveForward();
                        nextState = LightState;
                    }
                    makeTransition = TRUE;
                    break;
            }
            break;

        case LightState:
            switch (ThisEvent.EventType) {
                case DARK:
                    stop();
                    nextState = DarkState;
                    makeTransition = TRUE;
                    break;
                case FRONT_HIT:
                    nextState = AboutFaceState;
                    makeTransition = TRUE;
                    BackUp();
                    break;
                case FR_HIT:
                    if (escapeSeq){
                        escapeSeq = FALSE;
                        hellaRight = FALSE;
                        nextState = AboutFaceState;
                    }
                    else{
                        escapeSeq = TRUE;
                        nextState = LeftFaceState;
                    }
                    makeTransition = TRUE;
                    BackUp();
                    break;
                case FL_HIT:
                    if (escapeSeq){
                        escapeSeq = FALSE;
                        hellaRight = TRUE;
                        nextState = AboutFaceState;
                    }
                    else{
                        escapeSeq = TRUE;
                        nextState = RightFaceState;
                    }
                    makeTransition = TRUE;
                    BackUp();
                    break;
                case BACK_HIT:
                    nextState = AboutFaceState;
                    makeTransition = TRUE;
                    inchForward();
                    break;
                case RR_HIT:
                    nextState = LeftFaceState;
                    makeTransition = TRUE;
                    inchForward();
                    break;
                case RL_HIT:
                    nextState = RightFaceState;
                    makeTransition = TRUE;
                    inchForward();
                    break;
            }
            break;
            
        case DarkState:
            switch (ThisEvent.EventType) {
                case LIGHT:
                    moveForward();
                    nextState = LightState;
                    makeTransition = TRUE;
                    break;
                case FRONT_HIT:
                    nextState = AboutFaceState;
                    makeTransition = TRUE;
                    BackUp();
                    break;
                case FR_HIT:
                    if (escapeSeq){
                        escapeSeq = FALSE;
                        hellaRight = FALSE;
                        nextState = AboutFaceState;
                    }
                    else{
                        escapeSeq = TRUE;
                        nextState = LeftFaceState;
                    }
                    makeTransition = TRUE;
                    BackUp();
                    break;
                case FL_HIT:
                    if (escapeSeq){
                        escapeSeq = FALSE;
                        hellaRight = TRUE;
                        nextState = AboutFaceState;
                    }
                    else{
                        escapeSeq = TRUE;
                        nextState = RightFaceState;
                    }
                    makeTransition = TRUE;
                    BackUp();
                    break;
                case BACK_HIT:
                    nextState = AboutFaceState;
                    makeTransition = TRUE;
                    inchForward();
                    break;
                case RR_HIT:
                    nextState = LeftFaceState;
                    makeTransition = TRUE;
                    inchForward();
                    break;
                case RL_HIT:
                    nextState = RightFaceState;
                    makeTransition = TRUE;
                    inchForward();
                    break;
            }
            break;

        case AboutFaceState:
            switch (ThisEvent.EventType) {
                case DARK:
                    stop();
                    nextState = DarkState;
                    makeTransition = TRUE;
                    break;
                case ES_TIMEOUT:
                    if (hellaRight) TurnHellaRight();
                    else TurnHellaLeft();
                    nextState = CheckState; //goes back to the intial 
                    makeTransition = TRUE;
                    break;
            }
            break;

        case RightFaceState:
            switch (ThisEvent.EventType) {
                case DARK:
                    stop();
                    nextState = DarkState;
                    makeTransition = TRUE;
                    break;
                case ES_TIMEOUT:
                    TurnRight();
                    nextState = CheckState; //goes back to the intial 
                    makeTransition = TRUE;
                    break;
            }
            break;

        case LeftFaceState:
            switch (ThisEvent.EventType) {
                case DARK:
                    stop();
                    nextState = DarkState;
                    makeTransition = TRUE;
                    break;
                case ES_TIMEOUT:
                    TurnLeft();
                    nextState = CheckState; //goes back to the intial 
                    makeTransition = TRUE;
                    break;
            }
            break;


        case ForwardState:
            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State
    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunTemplateFSM(EXIT_EVENT);
        CurrentState = nextState;
        RunTemplateFSM(ENTRY_EVENT);
    }
    ES_Tail(); // trace call stack end
    return ThisEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
void stop(void) {
    //    printf("\n### SHOULD BE BACKING UP ###\n");
    //ES_Timer_InitTimer(SIMPLE_SERVICE_TIMER, 3000);
    Roach_LeftMtrSpeed(0);
    Roach_RightMtrSpeed(0);
    printf("\n>>> STOPPED\n");
}

void moveForward(void) {
    //    printf("\n### SHOULD BE MOVING UP ###\n");
    //    ES_Timer_InitTimer(SIMPLE_SERVICE_TIMER, 3000);

    Roach_LeftMtrSpeed(REG_SPEED);
    Roach_RightMtrSpeed(REG_SPEED);
    printf("\n>>> GO!\n");
}

uint8_t BackUp(void) {
    ES_Timer_InitTimer(BACKUP_TIMER, BACKUP_TIMER_TICKS);
    Roach_LeftMtrSpeed(-SLOW_SPEED);
    Roach_RightMtrSpeed(-SLOW_SPEED);
    printf("\n>>> BACKWARD\n");
    return TRUE;
}

uint8_t TurnLeft(void) {
    ES_Timer_InitTimer(BACKUP_TIMER, BACKUP_TIMER_TICKS);
    Roach_LeftMtrSpeed(-REG_TURN_SPEED);
    Roach_RightMtrSpeed(REG_TURN_SPEED);
    printf("\n>>> TURNING LEFT\n");
    return TRUE;
}

uint8_t TurnRight(void) {
    ES_Timer_InitTimer(BACKUP_TIMER, BACKUP_TIMER_TICKS);
    Roach_LeftMtrSpeed(REG_TURN_SPEED);
    Roach_RightMtrSpeed(-REG_TURN_SPEED);
    printf("\n>>> TURNING RIGHT\n");
    return TRUE;
}

uint8_t TurnHellaLeft(void) {
    ES_Timer_InitTimer(BACKUP_TIMER, BACKUP_TIMER_TICKS);
    Roach_LeftMtrSpeed(-FAST_TURN_SPEED);
    Roach_RightMtrSpeed(FAST_TURN_SPEED);
    printf("\n>>> TURNING HELLA\n");
    return TRUE;
}

uint8_t TurnHellaRight(void) {
    ES_Timer_InitTimer(BACKUP_TIMER, BACKUP_TIMER_TICKS);
    Roach_LeftMtrSpeed(FAST_TURN_SPEED);
    Roach_RightMtrSpeed(-FAST_TURN_SPEED);
    printf("\n>>> TURNING HELLA\n");
    return TRUE;
}

uint8_t inchForward(void) {
    ES_Timer_InitTimer(BACKUP_TIMER, BACKUP_TIMER_TICKS);
    Roach_LeftMtrSpeed(SLOW_SPEED);
    Roach_RightMtrSpeed(SLOW_SPEED);
    printf("\n>>> FORWARD\n");
    return TRUE;
}