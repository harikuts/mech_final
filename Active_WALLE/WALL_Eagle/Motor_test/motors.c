#include "xc.h"
#include <BOARD.h>
#include <stdio.h>
#include <string.h>
#include <AD.h>
#include <RC_Servo.h>
#include <LED.h>
#include <pwm.h>
#include <IO_Ports.h>
#include "PinsMaster.h"
#include "BatteryAdjust.h"

//#define LEFT_PWM_PIN PWM_PORTY10
//#define LEFT_DIR_PIN PORTY,PIN9
//#define RIGHT_PWM_PIN PWM_PORTY12
//#define RIGHT_DIR_PIN PORTY,PIN11

#define PWM_FREQUENCY 1000

void InitMotors(void){
    PWM_Init();
    PWM_AddPins(RIGHT_PWM_PIN);
    IO_PortsSetPortOutputs(RIGHT_DIR_PIN);
    PWM_AddPins(LEFT_PWM_PIN);
    IO_PortsSetPortOutputs(LEFT_DIR_PIN);
    PWM_SetFrequency(PWM_FREQUENCY);
}

void RightMotorMoveForward(int speed){
    speed = AdjustPWM(speed);
    PWM_SetDutyCycle(RIGHT_PWM_PIN, speed);
    IO_PortsClearPortBits(RIGHT_DIR_PIN);
}

void RightMotorMoveBackward(int speed){
    speed = AdjustPWM(speed);
    PWM_SetDutyCycle(RIGHT_PWM_PIN, speed);
    IO_PortsSetPortBits(RIGHT_DIR_PIN);
}

void RightMotorHalt(void){
    PWM_SetDutyCycle(RIGHT_PWM_PIN, 0);
    IO_PortsClearPortBits(RIGHT_DIR_PIN);
}

void LeftMotorMoveForward(int speed){
    speed = AdjustPWM(speed);
    PWM_SetDutyCycle(LEFT_PWM_PIN, speed);
    IO_PortsSetPortBits(LEFT_DIR_PIN);
}

void LeftMotorMoveBackward(int speed){
    speed = AdjustPWM(speed);
    PWM_SetDutyCycle(LEFT_PWM_PIN, speed);
    IO_PortsClearPortBits(LEFT_DIR_PIN);
}

void LeftMotorHalt(void){
    PWM_SetDutyCycle(LEFT_PWM_PIN, 0);
    IO_PortsClearPortBits(LEFT_DIR_PIN);
}

void BotHalt(void){
    LeftMotorHalt();
    RightMotorHalt();
}

void BotMoveForward(int speed){
    speed = AdjustPWM(speed);
    RightMotorMoveForward(speed);
    LeftMotorMoveForward(speed);
}

void BotMoveBackward(int speed){
    speed = AdjustPWM(speed);
    RightMotorMoveBackward(speed);
    LeftMotorMoveBackward(speed);
}

void BotTankTurnRight(int speed){
    speed = AdjustPWM(speed);
    RightMotorMoveBackward(speed);
    LeftMotorMoveForward(speed);
}

void BotTankTurnLeft(int speed){
    speed = AdjustPWM(speed);
    LeftMotorMoveBackward(speed);
    RightMotorMoveForward(speed);
}

void BotRPivotRight(int speed){
    speed = AdjustPWM(speed);
    int short_speed = speed/2;
    LeftMotorMoveForward(short_speed);
    RightMotorMoveBackward(speed);
}

void BotRPivotLeft(int speed){
    speed = AdjustPWM(speed);
    int short_speed = speed/2;
    RightMotorMoveForward(short_speed);
    LeftMotorMoveBackward(speed);
}

void BotRRPivotRight(int speed){
    speed = AdjustPWM(speed);
    int short_speed = speed/3;
    LeftMotorMoveBackward(short_speed);
    RightMotorMoveBackward(speed);
}

void BotRRPivotLeft(int speed){
    speed = AdjustPWM(speed);
    int short_speed = speed/3;
    RightMotorMoveBackward(short_speed);
    LeftMotorMoveBackward(speed);
}

void BotBankTurnLeft(int speed, int factor){
    speed = AdjustPWM(speed);
    int short_speed = speed / factor;
    LeftMotorMoveForward(short_speed);
    RightMotorMoveForward(speed);
}

void BotRBankTurnLeft(int speed, int factor){
    speed = AdjustPWM(speed);
    int short_speed = speed / factor;
    LeftMotorMoveBackward(short_speed);
    RightMotorMoveBackward(speed);
}

void BotBankTurnRight(int speed, int factor){
    speed = AdjustPWM(speed);
    int short_speed = speed / factor;
    RightMotorMoveForward(short_speed);
    LeftMotorMoveForward(speed);
}

void BotRBankTurnRight(int speed, int factor){
    speed = AdjustPWM(speed);
    int short_speed = speed / factor;
    RightMotorMoveBackward(short_speed);
    LeftMotorMoveBackward(speed);
}

void BotTrueRPivotLeft(int speed){
    speed = AdjustPWM(speed);
    RightMotorHalt();
    LeftMotorMoveBackward(speed);
}

void BotTrueRPivotRight(int speed){
    speed = AdjustPWM(speed);
    LeftMotorHalt();
    RightMotorMoveBackward(speed);
}

void BotInOrbit(){
    LeftMotorMoveForward(AdjustPWM(540));
    RightMotorMoveForward(AdjustPWM(210));
}
