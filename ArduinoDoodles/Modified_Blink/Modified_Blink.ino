void setup() {
  pinMode(42, OUTPUT);
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
