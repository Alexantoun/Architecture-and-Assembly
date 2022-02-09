/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(42, OUTPUT);//Initializes GPIO pin for "LED_BUILTIN" to be an output pin
  pinMode(43, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(49, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  for(int i =42; i<50; i++)
  {
    digitalWrite(i, HIGH);
  }
  for(int i =42; i<50; i++)
  {
    digitalWrite(i, LOW);
    
    delay(50);
    if(i!=49)
      digitalWrite(i, HIGH);
  }
  for(int i =50; i>41; i--)
  {
    digitalWrite(i, LOW);
    delay(50);
    digitalWrite(i, HIGH);
  }
                        // wait for parameter in milliseconds
}
