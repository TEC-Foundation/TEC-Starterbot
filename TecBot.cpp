#include "TecBot.h"

TecBot::TecBot(Servo *left, Servo *right)
{
  _leftMotor = left;
  _rightMotor = right;
  stop();
}

/**
 * Assigns the servo objects to control from TecBot.
 */
void TecBot::registerMotors(Servo *left, Servo *right)
{
  _leftMotor = left;
  _rightMotor = right;
}

/**
 * Drives two motors based on their individual powers.
 * Drive values are as follows:
 *   0 = full reverse
 *  90 = stop
 * 180 = full forward
 *
 * @param left The power to send to the left motor.
 * @param right The power to send to the right motor.
 * @param invLeft Whether to invert the left motor.
 * @param invRight Whether to invert the right motor.
 */
void TecBot::driveTank(int left, int right, bool invLeft, bool invRight)
{
  //Trim left and right motor values to a range of [0, 180].
  left = (left > 180) ? 180 : left;
  left = (left < 0) ? 0 : left;
  right = (right > 180) ? 180 : right;
  right = (right < 0) ? 0 : right;

  //Invert motor directions if needed.
  left = invLeft ? (180 - left) : left;
  right = invRight ? (180 - right) : right;

  (*_leftMotor).write(left);
  (*_rightMotor).write(right);
}

/**
 * Drives two motors based on a magnitude and rotation.
 * Magnitude values are as follows:
 *   0 = full reverse
 *  90 = stop
 * 180 = full forward
 *
 * Rotation values are as follows:
 *   0 = sharp left
 *  90 = straight
 * 180 = sharp right
 *
 * @param magnitude The base speed to drive the robot.
 * @param rotation The amount the robot should turn.
 * @param invDir Whether the direction of the robot should be inverted.
 * @param invTurn Whether the rotation should be inverted.
 */
void TecBot::driveArcade(int magnitude, int rotation, bool invDir, bool invTurn)
{
  //Trim drive values
  magnitude = (magnitude > 180) ? 180 : magnitude;
  magnitude = (magnitude < 0) ? 0 : magnitude;
  rotation = (rotation > 180) ? 180 : rotation;
  rotation = (rotation < 0) ? 0 : rotation;

  //Center the rotation value about 0 to be [-90, 90].
  rotation -= 90;

  //Invert drive values if necessary.
  magnitude = invDir ? (180 - magnitude) : magnitude;
  rotation = invTurn ? -rotation : rotation;

  //Calculate left and right speeds from magnitude and rotation.
  int left = magnitude + rotation;
  int right = magnitude - rotation;

  //Send left and right drive values to the Tank Drive.
  driveTank(left, right);
}

/**
 * Stops the drive motors of the robot.
 */
void TecBot::stop()
{
  (*_leftMotor).write(DRIVE_STOP);
  (*_rightMotor).write(DRIVE_STOP);
}
