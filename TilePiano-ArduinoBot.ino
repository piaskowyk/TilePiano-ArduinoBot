#define DEBUG 1

#define false 0
#define true 1

#define detector1Pin A0
#define detector2Pin A1
#define touchPin 6
#define buttonPin 2

volatile byte programState = false;
int detector1Value = 0;
int detector2Value = 0;
unsigned long timeStamp = 0;
int interruptInterval = 500;

void setup() {
  Serial.begin(9600);
  pinMode(touchPin, OUTPUT);

  timeStamp = millis() - interruptInterval;
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), bottonAction, FALLING);
}

void loop() {
  if(!programState) return;

  Serial.print("Detector 1: ");
  detector1Value = analogRead(A0);
  Serial.println(detector1Value);

  Serial.print("Detector 2: ");
  detector2Value = analogRead(A1);
  Serial.println(detector2Value);
  
  Serial.println("");
  delay(1000);
}

void bottonAction() {
  if(millis() - timeStamp <= interruptInterval) return;
  timeStamp = millis();
  programState = !programState;
  
  #if DEBUG
    Serial.print("Button press ");
    Serial.println(programState);
  #endif
}
