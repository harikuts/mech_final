/* 
 * File:   motors.h
 * Author: hkuttive
 *
 * Created on November 9, 2016, 4:57 PM
 */


#ifndef MOTORS_H
#define	MOTORS_H

#ifdef	__cplusplus
extern "C" {
#endif


    void InitMotors(void);
    void RightMotorMoveForward(int speed);
    void RightMotorMoveBackward(int speed);
    void RightMotorHalt(void);
    void LeftMotorMoveForward(int speed);
    void LeftMotorMoveBackward(int speed);
    void LeftMotorHalt(void);
    void BotMoveForward(int speed);
    void BotMoveBackward(int speed);
    void BotTankTurnRight(int speed);
    void BotTankTurnLeft(int speed);

#ifdef	__cplusplus
}
#endif

#endif	/* MOTORS_H */

