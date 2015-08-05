#ifndef TECBOT_H
#define TECBOT_H
#endif

#include <Servo.h>

class TecBot
{
public:
  TecBot(Servo *left, Servo *right);
  void registerMotors(Servo *left, Servo *right);
  void driveTank(int left, int right, bool invLeft = false, bool invRight = true);
  void driveArcade(int magnitude, int rotation, bool invDir = false, bool invTurn = false);
  void stop();
  void setTrim(int left, int right);
  void setTrim(int trim);

private:
  Servo *_leftMotor;
  Servo *_rightMotor;
  int leftTrim;
  int rightTrim;
};
