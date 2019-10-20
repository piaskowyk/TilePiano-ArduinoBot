#define DEBUG 1

#define GAME_MODE 0

#define calibrateButton 2
#define startButton 3

int interruptInterval = 500;
int calibrateState = 0;
int programState = 0;
int detectorTrashold = 15;

unsigned long timestampCalibrateButton = 0;
unsigned long timestampStateButton = 0;

int blackBlockAcceptable[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int clockAcceptableShift = 0;

int detectorsPin[4] = {A0, A1, A2, A3};
int touchsPin[4] = {4, 5, 6, 7};
int sensorsBar[2] = {8, 9};

void calibrate() {
  //TODO: dostosować do nowego sposobu kalibracji
  calibrateState = 1;
  int sumSensorValue = 0;
  int sensorMeansurmentCount = 5;

  selectSensorBar(0);
  for(int detectorNumber = 0; detectorNumber < 4; detectorNumber++) {
    for(int i = 0; i < sensorMeansurmentCount; i++) {
      sumSensorValue += analogRead(detector);
      delay(10);
    }
    whiteBlockAcceptable[detectorNumber + blockAcceptableShift] = sumSensorValue / sensorMeansurmentCount;
  }

  selectSensorBar(1);
  for(int detectorNumber = 0; detectorNumber < 4; detectorNumber++) {
    for(int i = 0; i < sensorMeansurmentCount; i++) {
      sumSensorValue += analogRead(detector);
      delay(10);
    }
    whiteBlockAcceptable[detectorNumber + blockAcceptableShift] = sumSensorValue / sensorMeansurmentCount;
  }

  click(0);
  calibrateState = 0;
}

void calibrateAction() {
  if(millis() - timestampCalibrateButton <= interruptInterval || calibrateState == 1) return;
  timestampCalibrateButton = millis();
  
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
  if(millis() - timestampStateButton <= interruptInterval) return;
  timestampStateButton = millis();
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
  timestampStateButton = millis() - interruptInterval;

  pinMode(calibrateButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(calibrateButton), calibrateAction, FALLING);

  pinMode(startButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(startButton), stateAction, FALLING);

  selectSensorBar(0);
}

void selectSensorBar(int barId) {
  if(barId == 0) {
    digitalWrite(sensorsBar[0], HIGH);
    digitalWrite(sensorsBar[1], LOW);
    blockAcceptableShift = 0;
  }
  else {
    digitalWrite(sensorsBar[1], HIGH);
    digitalWrite(sensorsBar[0], LOW);
    blockAcceptableShift = 4;
  }
}

//classic
void click_classic(int touchPin) {
  #if DEBUG
    Serial.print("Click ");
    Serial.println(touchPin);
  #endif

  touchPin = touchsPin[touchPin];
  digitalWrite(touchPin, HIGH);
  delay(10);
  digitalWrite(touchPin, LOW);
}

int detect_classic() {
  for(int i = 0; i < 4; i++) {
    if(analogRead(detectorsPin[i]) - blackBlockAcceptable[i + blockAcceptableShift] < detectorTrashold) {
      #if DEBUG
        Serial.print("Detected ");
        Serial.println(detectedTile);
      #endif
      return i;
    }
  }
  #if DEBUG
    Serial.println("Error, not detected!");
  #endif
}

//arcade
#define tickCountTo_calculateSpeed 10
#define distanceBetweenDetectorBars = 200

unsigned long detectedTime = 0;
unsigned long timeToWait = 300;
unsigned int counter = 0;
int detectBlock = 0;

int speedCalculateStart = false;
int observedTile = -1;
int blackTile = -1;
unsigned long speedTimestamp = 0;
int sensorDistance = 200;
float magicNumber = 1.1;

void click_arcade(int touchPin) {
  #if DEBUG
    Serial.print("Click ");
    Serial.println(touchPin);
  #endif

  touchPin = touchsPin[touchPin];
  digitalWrite(touchPin, HIGH);
  delay(10);
  digitalWrite(touchPin, LOW);
  detectBlock = 0;
}

int detect_arcade() {
  if(detectBlock) return -1;

  for(int i = 0; i < 4; i++) {
    if(analogRead(detectorsPin[i]) - blackBlockAcceptable[i + blockAcceptableShift] < detectorTrashold) {
      #if DEBUG
        Serial.print("Detected ");
        Serial.println(detectedTile);
      #endif
      detectBlock = 1;
      detectedTime = millis();

      counter++;
      if(!speedCalculateStart && counter % tickCountTo_calculateSpeed == 0) {
        speedCalculateStart = true;
      }

      calculateSpeed();

      return i;
    }
  }
  #if DEBUG
    Serial.println("Error, not detected!");
  #endif
}

int detectSimple() {
  for(int i = 0; i < 4; i++) {
    if(analogRead(detectorsPin[i]) - blackBlockAcceptable[i + blockAcceptableShift] < detectorTrashold) {
      #if DEBUG
        Serial.print("Detected ");
        Serial.println(detectedTile);
      #endif
      return i;
    }
  }
  #if DEBUG
    Serial.println("Error, not detected!");
  #endif
}

int isBlack(int tileNumber) {
  if(analogRead(detectorsPin[tileNumber]) - blackBlockAcceptable[tileNumber + blockAcceptableShift] < detectorTrashold) {
      #if DEBUG
        Serial.print("Detected BLACK for tile");
        Serial.println(tileNumber);
      #endif
      return true;
    }
  #if DEBUG
    Serial.print("Detected WHITE for tile");
    Serial.println(tileNumber);
  #endif
  return false;
}

void calculateSpeed() {
  //ignore calculate
  if(!speedCalculateStart) {
    observedTile = -1;
    return;
  }

  //detect white tile
  if(observedTile == -1) {
    selectSensorBar(1);
    observedTile = detectSimple() + 1 % 4;
    selectSensorBar(0);
    return;
  }

  if(speedTimestamp != 0) {
    //wait for white tile change in black
    selectSensorBar(1);
    if(isBlack(observedTile)) {
      //start mensurement
      speedTimestamp = millis();
    }
    selectSensorBar(0);
    return;
  }
  
  if(isBlack(observedTile)) {
    timeToWait = (millis() - speedTimestamp) * magicNumber;
    speedCalculateStart = 0;
  }
}

void loop() {
  if(!programState) return;

  //clasic mode
  if(GAME_MODE == 0) {
    int currentTile = 0;
    int nextTile = 3;

    while(programState) {
      currentTile = nextTile;
      nextTile = detect_classic();
      click_classic(currentTile);
    }
  }

  //arcade mode
  if(GAME_MODE == 1) {
    int detectedResult = 0;
    int currentTile;
    int nextTile;

    Serial.print("analize_first");
    while(true) {
      if(Serial.available() > 0) {
        currentTile = Serial.read();
        break;
      }
      delay(100);
    }

    while(programState) {
      detectedResult = detect_arcade();
      if(detectedResult >= 0) {
        nextTile = detectedResult;
      }

      if(millis() - detectedTime > timeToWait) {
        click_arcade(currentTile);
        currentTile = nextTile;
      }
    }
  }
}