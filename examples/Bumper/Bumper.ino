//Declare pins which are wired to Limit Switches.
const int BUMPER_LEFT_PIN = 0;
const int BUMPER_RIGHT_PIN = 1;

//Run once to initialize code.
void setup()
{
  //Set up Serial Monitor.
  Serial.begin(9600);

  //Let Arduino know that these pins are inputs.
  pinMode(BUMPER_LEFT_PIN, INPUT);
  pinMode(BUMPER_RIGHT_PIN, INPUT);
}

//Run continuously forever.
void loop()
{
  Serial.print("Left Bumper Pressed: ");

  //Detect the value of the limit switches.
  Serial.print(digitalRead(BUMPER_LEFT_PIN) ? "True" : "False");
  Serial.print("\tRight Bumper Pressed: ");
  Serial.print(digitalRead(BUMPER_RIGHT_PIN) ? "True" : "False");

  /*
   * The bare minimum way to read a pin's value is:
   *
   *    digitalRead(PIN_NUMBER);
   *
   * The other portion of the above code is called 'ternary' :?
   * which switches value based on a boolean (true or false) value.
   */

  Serial.println();
}
