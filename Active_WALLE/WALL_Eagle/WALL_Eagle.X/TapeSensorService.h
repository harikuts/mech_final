/*
 * File: TemplateFSM.h
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 *
 * Template file to set up a Flat State Machine to work with the Events and Services
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that this file
 * will need to be modified to fit your exact needs, and most of the names will have
 * to be changed to match your code.
 *
 * This is provided as an example and a good place to start.
 *
 * Created on 23/Oct/2011
 * Updated on 16/Sep/2013
 * Modified by MaxL on 8/11/2014
 */

#ifndef TapeSensorService_H  // <- This should be changed to your own guard on both
#define TapeSensorService_H  //    of these lines


/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define WHITE_TAPE_THRESHOLD 800
#define BLACK_TAPE_THRESHOLD 400

//#define FAR_LEFT_TAPE_SENSOR_PIN AD_PORTW3
//#define MID_LEFT_TAPE_SENSOR_PIN AD_PORTW4
//#define MID_RIGHT_TAPE_SENSOR_PIN AD_PORTW5
//#define FAR_RIGHT_TAPE_SENSOR_PIN AD_PORTW6
//#define FLASH_PIN PORTW,PIN7
#define FLASH_ON IO_PortsSetPortBits(FLASH_PIN)
#define FLASH_OFF IO_PortsClearPortBits(FLASH_PIN)

/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/
typedef enum {
    SAMPLE_FLASH_ON,
    SAMPLE_TAKE_SAMPLE_1,
    SAMPLE_FLASH_OFF,
    SAMPLE_TAKE_SAMPLE_2,
} sampleState;

typedef enum {
    BLACK,
    WHITE
} tape_status;

typedef struct tape_sensor tape_sensor;

struct tape_sensor {
    unsigned int sample1;
    unsigned int sample2;
    unsigned int diff;
    tape_status status;
    tape_status new_status;
};


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
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
uint8_t InitTapeSensorService(uint8_t Priority);

/**
 * @Function PostTemplateFSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostTapeSensorService(ES_Event ThisEvent);


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
ES_Event RunTapeSensorService(ES_Event ThisEvent);

uint8_t dummy_func(void);

void TakeFirstSample(void);
void TakeSecondSample(void);
void UpdateStatus(void);
void AssembleReadings(void);

#endif /* TapeSensorService_H */

