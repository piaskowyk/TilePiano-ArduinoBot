#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

String input = "";

void setup() {
  Serial.begin(9600);

  servo1.attach(4);
  servo1.write(40); // 20
  
  servo2.attach(5);
  servo2.write(20); //41

  servo3.attach(6);
  servo3.write(50); // 29

  servo4.attach(7);
  servo4.write(25); // 40
}

void loop() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      input += (char)inChar;
    }

    if (inChar == '\n') {
      servo4.write(input.toInt());
      Serial.println(input);
      input = "";
      inChar = 0;
    }
  }
}