#include "xc.h"
#include <BOARD.h>
#include <stdio.h>
#include <string.h>
#include <AD.h>
#include <RC_Servo.h>
#include <LED.h>
#include <pwm.h>
#include <IO_Ports.h>

#define RIGHT_PWM_PIN PWM_PORTY10
#define RIGHT_DIR_PIN PORTY,PIN9
#define LEFT_PWM_PIN PWM_PORTY12
#define LEFT_DIR_PIN PORTY,PIN11

#define PWM_FREQUENCY 1000

void InitMotors(void){
    PWM_AddPins(RIGHT_PWM_PIN);
    IO_PortsSetPortOutputs(RIGHT_DIR_PIN);
    PWM_AddPins(LEFT_PWM_PIN);
    IO_PortsSetPortOutputs(LEFT_DIR_PIN);
    PWM_SetFrequency(PWM_FREQUENCY);
}

void RightMotorMoveForward(int speed){
    PWM_SetDutyCycle(RIGHT_PWM_PIN, speed);
    IO_PortsClearPortBits(RIGHT_DIR_PIN);
}

void RightMotorMoveBackward(int speed){
    PWM_SetDutyCycle(RIGHT_PWM_PIN, speed);
    IO_PortsSetPortBits(RIGHT_DIR_PIN);
}

void RightMotorHalt(void){
    PWM_SetDutyCycle(RIGHT_PWM_PIN, 0);
    IO_PortsClearPortBits(RIGHT_DIR_PIN);
}

void LeftMotorMoveForward(int speed){
    PWM_SetDutyCycle(LEFT_PWM_PIN, speed);
    IO_PortsSetPortBits(LEFT_DIR_PIN);
}

void LeftMotorMoveBackward(int speed){
    PWM_SetDutyCycle(LEFT_PWM_PIN, speed);
    IO_PortsClearPortBits(LEFT_DIR_PIN);
}

void LeftMotorHalt(int speed){
    PWM_SetDutyCycle(LEFT_PWM_PIN, 0);
    IO_PortsClearPortBits(LEFT_DIR_PIN);
}

