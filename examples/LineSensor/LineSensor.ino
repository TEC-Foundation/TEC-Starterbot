#include <QTRSensors.h>

const int LIGHT1 = 0;
const int LIGHT2 = 1;
const int LIGHT3 = 2;

//Constructs an Analog line sensor.
QTRSensorsAnalog lineSensor((unsigned char[]){LIGHT1, LIGHT2, LIGHT3}, 3, 4, 2);

//Creates an array to store individual light sensor readings in.
unsigned int sensorValues[3];

void setup()
{
  Serial.begin(9600);

  //For the first 5 seconds of execution, calibrate the light sensor.
  long time = millis();
  while(millis() - time < 5000)
  {
    lineSensor.calibrate();
  }
}

void loop()
{
  //Read individual light readings into the 'sensorValues' array.
  lineSensor.read(sensorValues);

  //Print the values of each light sensor.
  Serial.print("Sensor1: ");
  Serial.print(sensorValues[0]);
  Serial.print("\tSensor2: ");
  Serial.print(sensorValues[1]);
  Serial.print("\tSensor3: ");
  Serial.print(sensorValues[2]);

  //Print the value of the LINE.
  Serial.print("\tLine Value: ");

  int lineValue = lineSensor.readLine(sensorValues);

  Serial.print(lineValue);
  Serial.println();
}
