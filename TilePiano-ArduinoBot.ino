#include <Servo.h>

#define DEBUG 1
#define FORCE_CALIBRATION 1
#define MANUAL_INPUT 0

#define GAME_MODE 0

#define calibrateButton 2
#define startButton 3
#define DETECTOR_TRASHOLD 3
#define INTERRUPT_INVERTAL 500
#define SERVO_DELAY 100

int calibrateState = 0;
int programState = 0;

String input = "";

unsigned long timestampCalibrateButton = 0;
unsigned long timestampStateButton = 0;

int blackBlockAcceptable[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int blockShift = 0;

int detectorsPin[4] = {A0, A1, A2, A3};
int sensorsBar[2] = {8, 9};

Servo touchsServo[4];
int servoPin[4] = {4, 5, 6, 7};
int servoTopPosition[4] = {35, 30, 50, 25};
int servoBottomPosition[4] = {8, 59, 24, 55};

unsigned long detectedTime = 0;
unsigned long timeToWait = 150;

void setup() {
  Serial.begin(9600);

  for(int pin : sensorsBar) {
    pinMode(pin, OUTPUT);
  }

  int i = 0;
  for(Servo servo : touchsServo) {
    servo.attach(servoPin[i]);
    servo.write(servoTopPosition[i]);
    i++;
  }

  timestampCalibrateButton = millis() - INTERRUPT_INVERTAL;
  timestampStateButton = millis() - INTERRUPT_INVERTAL;

  pinMode(calibrateButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(calibrateButton), calibrateAction, FALLING);

  pinMode(startButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(startButton), stateAction, FALLING);

  selectSensorBar(0);

  #if FORCE_CALIBRATION
    for(int i = 0; i < 8; i++) {
      blackBlockAcceptable[i] = 10;
    }
  #endif

  #if DEBUG
    Serial.println("Initilization: OK");
  #endif

  Serial.println("START!");
}

void loop() {
  if(!programState) return;

  //clasic mode
  if(GAME_MODE == 0) {
    int currentTile = 2;
    int nextTile = 2;
    int detected = -1;

    while(programState) {
      
      detected = detect_classic();
      
      if(detected > -1) {
        currentTile = nextTile;
        nextTile = detected;
      }

      click_classic(currentTile);
//      delay(500);
      
      #if MANUAL_INPUT
        int inChar = 'a';
        while (Serial.available() > 0 || inChar != '\n') {
          inChar = Serial.read();
          if (isDigit(inChar)) {
            input += (char)inChar;
          }
      
          if (inChar == '\n') {
            inChar = 0;
            break;
          }
        }
        click_classic(input.toInt());
        input = "";
      #endif

      #if DEBUG and MANUAL_INPUT == 0
        delay(500);
        Serial.println("");
      #endif
    }
  }
}

// =======================================================================
/* config and tools */
// =======================================================================
void testDetector() {
  selectSensorBar(0);
  for(int i = 0; i < 4; i++) {
    Serial.print(analogRead(detectorsPin[i]));
    Serial.print(" | ");
  }
  Serial.println();
  selectSensorBar(1);
  for(int i = 0; i < 4; i++) {
    Serial.print(analogRead(detectorsPin[i]));
    Serial.print(" | ");
  }
  Serial.println();
  selectSensorBar(0);
}

void calibrate() {
  calibrateState = 1;
  int sumSensorValue = 0;
  int sensorMeasurmentCount = 5;

  selectSensorBar(0);
  for(int detectorNumber = 0; detectorNumber < 8; detectorNumber++) {
    if(detectorNumber == 4) {
      selectSensorBar(1);
    }

    sumSensorValue = 0;
    for(int i = 0; i < sensorMeasurmentCount; i++) {
      sumSensorValue += analogRead(detectorsPin[i]);
      delay(50);
    }
    blackBlockAcceptable[detectorNumber + blockShift] = sumSensorValue / sensorMeasurmentCount;
    click_classic(0);
  }

  calibrateState = 0;
}

void calibrateAction() {
  if(millis() - timestampCalibrateButton <= INTERRUPT_INVERTAL || calibrateState == 1) return;
  timestampCalibrateButton = millis();

  #if DEBUG
    Serial.println("Calibrate START!");
  #endif

  calibrate();

  #if DEBUG
    Serial.println("Calibrate END!");
  #endif
}

void stateAction() {
  if(millis() - timestampStateButton <= INTERRUPT_INVERTAL) return;
  timestampStateButton = millis();
  programState = !programState;
  
  #if DEBUG
    Serial.print("Button press, program state: ");
    Serial.println(programState);
  #endif
}

void selectSensorBar(int barId) {
  if(barId == 0) {
    digitalWrite(sensorsBar[0], HIGH);
    digitalWrite(sensorsBar[1], LOW);
    blockShift = 0;
  }
  else {
    digitalWrite(sensorsBar[1], HIGH);
    digitalWrite(sensorsBar[0], LOW);
    blockShift = 4;
  }
}

// =======================================================================
/* classic */
// =======================================================================

int barDetect[2] = {-1, -1};
int lastBarDetect[2] = {-1, -1};

void click_classic(int servoIndex) {
  #if DEBUG
    Serial.print("Click ");
    Serial.println(servoIndex);
  #endif

  touchsServo[servoIndex].write(servoBottomPosition[servoIndex]);
  delay(SERVO_DELAY);
  touchsServo[servoIndex].write(servoTopPosition[servoIndex]);
}

int detect_classic() {

  #if DEBUG
    testDetector();
  #endif

  lastBarDetect[0] = barDetect[0];
  lastBarDetect[1] = barDetect[1];

  for(int barIndex = 0; barIndex < 2; barIndex++) {
    selectSensorBar(barIndex);
    for(int i = 0; i < 4; i++) {
      if(analogRead(detectorsPin[i]) - blackBlockAcceptable[i + blockShift] < DETECTOR_TRASHOLD) {
        #if DEBUG
          Serial.print("Detected ");
          Serial.println(i);
        #endif
  
        barDetect[barIndex] = i;
        break;
      }
    } 
  }

  if(barDetect[0] != lastBarDetect[1] && lastBarDetect[0] != -1) {
    #if DEBUG
      Serial.println("NOT TAP: replay!");
    #endif
    return -1;
  }

  return barDetect[0];
}
