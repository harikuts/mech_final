/* 
 * File:   PinsMaster.h
 * Author: hkuttive
 *
 * Created on November 16, 2016, 10:00 PM
 */
#include "AD.h"
#include "pwm.h"
#include "IO_Ports.h"
#include "RC_Servo.h"

#ifndef PINSMASTER_H
#define	PINSMASTER_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Define all pins below

    // Bumper Pins
#define LEFT_BUMPER_PIN AD_PORTV3
#define RIGHT_BUMPER_PIN AD_PORTV4
    // Beacon Detector Pin
#define BEACON_DETECTOR_PIN AD_PORTV8
    // Track Wire Detection Pins
#define TW_DETECTOR_0_PIN AD_PORTV5
#define TW_DETECTOR_1_PIN AD_PORTV6
    // Motor Pins
#define LEFT_PWM_PIN PWM_PORTY10
#define LEFT_DIR_PIN PORTY,PIN9
#define RIGHT_PWM_PIN PWM_PORTY12
#define RIGHT_DIR_PIN PORTY,PIN11
    // Tape Sensor Pins
#define FAR_LEFT_TAPE_SENSOR_PIN AD_PORTW3
#define MID_LEFT_TAPE_SENSOR_PIN AD_PORTW4
#define MID_RIGHT_TAPE_SENSOR_PIN AD_PORTW5
#define FAR_RIGHT_TAPE_SENSOR_PIN AD_PORTW6
#define FLASH_PIN PORTW,PIN7 
    // Servo Pins
#define SERVO_0_OUT_PIN RC_PORTY06
#define SERVO_1_OUT_PIN RC_PORTY07

    // AD_Init List
#define AD_PIN_LIST (LEFT_BUMPER_PIN|RIGHT_BUMPER_PIN|BEACON_DETECTOR_PIN|TW_DETECTOR_0_PIN|TW_DETECTOR_1_PIN|FAR_LEFT_TAPE_SENSOR_PIN|MID_LEFT_TAPE_SENSOR_PIN|MID_RIGHT_TAPE_SENSOR_PIN|FAR_RIGHT_TAPE_SENSOR_PIN)

    // PWM Init List
#define PWM_PIN_LIST (LEFT_PWM_PIN|RIGHT_PWM_PIN)

    // RC Init List
#define RC_PIN_LIST (SERVO_0_OUT_PIN|SERVO_1_OUT_PIN)

#ifdef	__cplusplus
}
#endif

#endif	/* PINSMASTER_H */

