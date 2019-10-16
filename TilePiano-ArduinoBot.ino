#define DEBUG 1

#define GAME_MODE 0

#define calibrateButton 2
#define startButton 3

int interruptInterval = 500;
int calibrateState = 0;
int programState = 0;
int detectorTrashold = 15;

unsigned long timestampCalibrateButton = 0;
unsigned long timestampStartButton = 0;

int blackBlockAcceptable[4] = {0, 0, 0, 0};
int whiteBlockAcceptable[4] = {0, 0, 0, 0};

int detectorsPin[4] = {A0, A1, A2, A3};
int touchsPin[4] = {4, 5, 6, 7}; 

void calibrate() {
  calibrateState = 1;
  int sumSensorValue = 0;
  int sensorMeansurmentCount = 5;

  for(int detectorNumber = 0; detectorNumber < 4; detectorNumber++) {
    for(int i = 0; i < sensorMeansurmentCount; i++) {
      sumSensorValue += analogRead(detector);
      delay(10);
    }
    whiteBlockAcceptable[detectorNumber] = sumSensorValue / sensorMeansurmentCount;
  }

  click(0);

  for(int detectorNumber = 0; detectorNumber < 4; detectorNumber++) {
    for(int i = 0; i < sensorMeansurmentCount; i++) {
      sumSensorValue += analogRead(detector);
      delay(10);
    }
    blackBlockAcceptable[detectorNumber] = sumSensorValue / sensorMeansurmentCount;
  }

  click(0);
  calibrateState = 0;
}

void calibrateAction() {
  if(millis() - timestampCalibrateButton <= interruptInterval || calibrateState == 1) return;
  timeStamp = millis();
  
  #if DEBUG
    Serial.print("Button press ");
    Serial.println(programState);
  #endif

  #if DEBUG
    Serial.println("Calibrate START!");
  #endif

  calibrate();

  #if DEBUG
    Serial.println("Calibrate END!");
  #endif
}

void stateAction() {
  if(millis() - timestampCalibrateButton <= interruptInterval) return;
  timeStamp = millis();
  programState = !programState;
  
  #if DEBUG
    Serial.print("Button press ");
    Serial.println(programState);
  #endif
}

void setup() {
  Serial.begin(9600);

  for(int pin : touchsPin) {
    pinMode(pin, OUTPUT);
  }

  timestampCalibrateButton = millis() - interruptInterval;
  timestampStartButton = millis() - interruptInterval;

  pinMode(calibrateButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(calibrateButton), calibrateAction, FALLING);

  pinMode(startButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(startButton), stateAction, FALLING);
}

void click(int touchPin) {
  #if DEBUG
    Serial.print("Click ");
    Serial.println(touchPin);
  #endif

  touchPin = touchsPin[touchPin];
  digitalWrite(touchPin, HIGH);
  delay(10);
  digitalWrite(touchPin, LOW);
}

int detect() {
  int detectedTile = 0;

  for(int i = 0; i < 4; i++) {
    if(analogRead(detectorsPin[i]) - blackBlockAcceptable[i] < detectorTrashold || ) {
      detectedTile = i;
      break;
    }
  }

  #if DEBUG
    Serial.print("Detected ");
    Serial.println(detectedTile);
  #endif

  return detectedTile;
}

void loop() {
  if(!programState) return;

  //clasic mode
  if(GAME_MODE == 0) {
    int currentTile = 0;
    int nextTile = 3;

    while(programState) {
      currentTile = nextTile;
      nextTile = detect();
      click(currentTile);
    }
  }

  //arcade mode
  if(GAME_MODE == 1) {
    //TODO: to imlement
  }
}