#define DEBUG 1

#define false 0
#define true 1

#define detector1Pin A0
#define detector2Pin A1
#define detector1Pin A2
#define detector2Pin A3

#define touchPin 6
#define buttonPin 2

volatile byte programState = false;
int detector1Value = 0;
int detector2Value = 0;
unsigned long timeStamp = 0;
int interruptInterval = 500;

////////////////////////////////////

int blackBlock = 0;
int whiteBlock = 0;

int detectorsPin[4] = {A0, A1, A2, A3};
int touchsPin[4] = {4, 5, 6, 7}; 

////////////////////////////////////

void setup() {
  Serial.begin(9600);

  for(int pin : touchsPin) {
    pinMode(pin, OUTPUT);
  }

  timeStamp = millis() - interruptInterval;
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), bottonAction, FALLING);

  //calibrate sensor with app
  int sumSensorValue = 0;
  int sensorMeansurmentCount = 5;
  for(int i = 0; i < sensorMeansurmentCount; i++) {
    sumSensorValue += analogRead(detector1Pin);
  }
  whiteBlock = sumSensorValue / sensorMeansurmentCount;

}

void loop() {
  if(!programState) return;

  Serial.print("Detector 1: ");
  detector1Value = analogRead(detector1Pin);
  Serial.println(detector1Value);

  Serial.print("Detector 2: ");
  detector2Value = analogRead(detector2Pin);
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
