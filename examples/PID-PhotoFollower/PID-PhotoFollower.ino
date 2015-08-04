#include "PID.h"
#include "TecBot.h"
#include <Servo.h>
#include <QTRSensors.h>

//Motor pins.
const int LEFT_MOTOR_PIN = 10;
const int RIGHT_MOTOR_PIN = 9;

//Light sensor pins.
const int LIGHT1 = 0;
const int LIGHT2 = 1;
const int LIGHT3 = 2;

//Global drive variables
int driveMagnitude = 90;
int steerMagnitude = 90;

//PID handling functions.
void pidOutput(int output);
int pidSource();

//The PID system in this example is used to steer the robot left and right.
PIDController steerController(0.03, 0.0, 0.0, pidSource, pidOutput);//0.05, 0.0003, 0.8

//Light sensor array for line following.
QTRSensorsAnalog photoSensor((unsigned char[]){LIGHT1, LIGHT2, LIGHT3}, 3, 4, 2);

//Array holds individual light sensor values
unsigned int sensorValues[3];

//Declare motor objects using the Servo library.
Servo leftMotor;
Servo rightMotor;

//Create a TecBot object using existing Servos.
TecBot myBot(&leftMotor, &rightMotor);

void setup()
{
  Serial.begin(9600);
  leftMotor.attach(LEFT_MOTOR_PIN);
  rightMotor.attach(RIGHT_MOTOR_PIN);

  steerController.setOutputBounds(-90, 90);
  steerController.setTarget(1000);
  steerController.registerTimeFunction(millis);

  calibrateLineSensor();
}

void loop()
{
  steerController.tick();

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
  
  myBot.driveArcade(driveMagnitude, steerMagnitude);
}

void pidOutput(int output)
{
  steerMagnitude = DRIVE_STRAIGHT + output;
}

int pidSource()
{
  return photoSensor.readLine(sensorValues);
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
      photoSensor.calibrate();
    }
    //Reverse motor direction.
    temp = left; left = right; right = temp;
  }
  myBot.driveTank(right, left);
  time = millis();
  while(millis() - time < 125)
  {
    photoSensor.calibrate();
  }
  
  myBot.stop();
}
