#include <Wire.h>
#include <LSM303.h>
#include "PID.h"
#include "TecBot.h"
#include "Timer.h"
#include <Servo.h>

//Declare motor pins.
const int LEFT_MOTOR_PIN = 9;
const int RIGHT_MOTOR_PIN = 10;

int direction = 0;

//Declare servo motors.
Servo leftMotor;
Servo rightMotor;

//Declare TecBot with existing servos.
TecBot myBot(&leftMotor, &rightMotor);

//Declare a timer object that can be used for non-blocking waits.
Timer myTimer(millis);
int timerIndex = 0;

//Declare a compass object to read angle position.
LSM303 compass;

//Declare PID input/output handling functions (defined below).
int pidSource();
void pidOutput(int output);

//Declare a PIDController to manage turning.
PIDController steerController(0.25, 0.0, 0.0, pidSource, pidOutput); //P, I, D

//Global variables that control the robot's drive.
int driveMagnitude = 90;
int steerMagnitude = 90;

//Run once at the beginning to initialize.
void setup()
{
  //Initialize Serial Monitor
  Serial.begin(9600);

  //Initialize communication for compass.
  Wire.begin();

  //Initialize compass.
  compass.init();
  compass.enableDefault();
  compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};

  //Initialize steerController.
  steerController.setOutputBounds(-90, 90); //Limits output range. Min -90, Max 90.
  steerController.setFeedbackWrapBounds(0, 360); //Wraps 360 degrees back to 0 and vice versa
  steerController.setTarget(0);
  steerController.registerTimeFunction(millis);

  //Associate motor pins with servo motors.
  leftMotor.attach(LEFT_MOTOR_PIN);
  rightMotor.attach(RIGHT_MOTOR_PIN);

  myBot.setTrim(10, 0);

  //myTimer.set(5000);
}

//Run continuously.
void loop()
{
  //Updates compass position.
  compass.read();

  /*
   * The case below will detect if the timer object has triggered.
   * When the timer triggers, you can use if() statements and the
   * timerIndex to tell which code should run next.  If code should
   * be regularly repeated (like below), then the timerIndex should
   * use a combination increment (+) and modulo (%) operator to 
   * cycle through numbers.
   */
  if(myTimer.isDone())
  {
    Serial.print("Timer done!");
    if(timerIndex == 0)
    {
      steerController.setTarget(120);
    }
    else if(timerIndex == 1)
    {
      steerController.setTarget(240);
    }
    else if(timerIndex == 2)
    {
      steerController.setTarget(0);
    }
    //Reset the timer for 5 seconds.
    myTimer.set(5000);

    //Cycle the timer index from 0 through 2.
    timerIndex = (timerIndex + 1) % 3;
  }

  //Runs PID calculations.
  steerController.tick();

  //Prints PID status
  Serial.print("Target: ");
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

int pidSource()
{
  //return (int) compass.heading();
  return 0;
}

void pidOutput(int output)
{
  steerMagnitude = 90 + output;
}
