#ifndef TECBOT_H
#define TECBOT_H
#endif

#include <Servo.h>

const int DRIVE_FORWARD = 180;
const int DRIVE_STOP = 90;
const int DRIVE_REVERSE = 0;

const int TURN_LEFT = 180;
const int DRIVE_STRAIGHT = 90;
const int TURN_RIGHT = 0;

class TecBot
{
public:
  TecBot(Servo *left, Servo *right);
  void registerMotors(Servo *left, Servo *right);
  void driveTank(int left, int right, bool invLeft = false, bool invRight = true);
  void driveArcade(int magnitude, int rotation, bool invDir = false, bool invTurn = false);
  void stop();

private:
  Servo *_leftMotor;
  Servo *_rightMotor;
};
