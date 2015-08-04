/*
 * Declare the pin of the Distance Sensor.  Note that on an analog sensor,
 * this means pin A6 on the pinout diagram, not pin 6.
 */
const int DISTANCE_PIN = 6;

//DECLARES a function called distance().  It is DEFINED below.
int distance();

void setup()
{
  //Initializes the Serial Monitor.
  Serial.begin(9600);

  //Sets the sensor pin to input mode.
  pinMode(DISTANCE_PIN, INPUT);
}

void loop()
{
  //Continuously prints the sensor value.
  Serial.print("Distance: ");
  Serial.println(distance());
  delay(200);
}

//DEFINES the distance function.  This returns the value from the analog sensor.
int distance()
{
  return analogRead(DISTANCE_PIN);
}
