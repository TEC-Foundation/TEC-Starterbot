#include "PID.h"
#include "TecBot.h"
#include <Servo.h>
#include <QTRSensors.h>

//Motor pins.
const int LEFT_MOTOR_PIN = 10;
const int RIGHT_MOTOR_PIN = 9;

//Light sensor pins.  For analog sensors, use 0 for A0, 1 for A1, etc.
const int LIGHT1 = 0;
const int LIGHT2 = 1;
const int LIGHT3 = 2;

/*
 * In this example, driveMagnitude never changes, so this number is the
 * constant forward speed of the robot when it runs.  90 means that the
 * robot will sit still and move only based on how it is directed to steer.
 */
int driveMagnitude = 90;

/*
 * In this example, the steerMagnitude is used in the pidOutput() function
 * as a way for the PIDController to influence the robot's movement.  It is
 * constantly updated, which allows the drive function in loop() to make
 * the robot turn toward the line.  If no driveMagnitude is specified, then
 * the robot will find the center of the line and sit still unless it is pushed
 * off.
 */
int steerMagnitude = 90;

//PID handling functions.
void pidOutput(int output);
int pidSource();

//The PID system in this example is used to steer the robot left and right.
PIDController steerController(0.03, 0.0, 0.0, pidSource, pidOutput);

//Light sensor array for line following.
QTRSensorsAnalog lineSensor((unsigned char[]){LIGHT1, LIGHT2, LIGHT3}, 3, 4, 2);

//Array holds individual light sensor values
unsigned int sensorValues[3];

//Declare motor objects using the Servo library.
Servo leftMotor;
Servo rightMotor;

//Create a TecBot object using existing Servos.
TecBot myBot(&leftMotor, &rightMotor);

void setup()
{
  //Initialize the Serial Monitor.
  Serial.begin(9600);

  //Assign pin values to the Servo motors.
  leftMotor.attach(LEFT_MOTOR_PIN);
  rightMotor.attach(RIGHT_MOTOR_PIN);

  //Sets output bounds for the PIDController which limits the min and max output.
  steerController.setOutputBounds(-90, 90);
  //Tells the PIDController where it should try to get to.
  steerController.setTarget(1000);
  //Tells the PIDController how to acquire the system time.
  steerController.registerTimeFunction(millis);

  calibrateLineSensor();
}

void loop()
{
  //Causes the PIDController to run its calculations and deliver its output.
  steerController.tick();

  //Prints information about the PID control loop's status.
  Serial.print("[GAINS -> P: ");
  Serial.print(steerController.getP());
  Serial.print(" I: ");
  Serial.print(steerController.getI());
  Serial.print(" D: ");
  Serial.print(steerController.getD());
  Serial.print("]");
  Serial.print("\tTarget: ");
  Serial.print(steerController.getTarget());
  Serial.print("\tFeedback: ");
  Serial.print(steerController.getFeedback());
  Serial.print("\tPComp: ");
  Serial.print(steerController.getProportionalComponent());
  Serial.print("\tIComp: ");
  Serial.print(steerController.getIntegralComponent());
  Serial.print("\tDComp: ");
  Serial.print(steerController.getDerivativeComponent());
  Serial.print("\tOutput: ");
  Serial.print(steerController.getOutput());
  Serial.println();

  //Drives the robot in arcade mode using global variables as values.
  myBot.driveArcade(driveMagnitude, steerMagnitude);
}

/**
 * After each tick() of the PIDController, the PIDController delivers
 * the generated output value through this pidOutput() function.  We
 * use this output to influence the global steering variable that
 * the robot uses in the loop.
 */
void pidOutput(int output)
{
  steerMagnitude = DRIVE_STRAIGHT + output;
}

/**
 * At the beginning of each tick() of the PIDController, the PIDController
 * fetches the sensor input with this pidSource() function.  In this
 * function, we "return" the value from our sensor so the PIDController can
 * use it.
 */
int pidSource()
{
  return lineSensor.readLine(sensorValues);
}

void calibrateLineSensor()
{
  long time;
  int left = DRIVE_FORWARD;
  int right = DRIVE_REVERSE;
  int temp;

  //Move to one side of the line.
  myBot.driveTank(right, left);
  delay(125);

  //Sweep across the line while calibrating.
  for(int i = 0; i < 4; i++)
  {
    myBot.driveTank(left, right);
    time = millis();
    while(millis() - time < 250)
    {
      //Calibrate line sensor
      lineSensor.calibrate();
    }
    //Reverse motor direction.
    temp = left; left = right; right = temp;
  }
  myBot.driveTank(right, left);
  time = millis();
  while(millis() - time < 125)
  {
    lineSensor.calibrate();
  }

  myBot.stop();
}
