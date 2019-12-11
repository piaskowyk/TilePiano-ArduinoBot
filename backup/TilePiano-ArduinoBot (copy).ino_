#define DEBUG 1

int detectorTrashold = 15;

int blackBlockAcceptable[8] = {20, 20, 20, 20, 0, 0, 0, 0};
int blockAcceptableShift = 0;

int detectorsPin[4] = {A0, A1, A2, A3};
int touchsPin[4] = {4, 5, 6, 7};
int sensorsBar[2] = {8, 9};

void setup() {
  Serial.begin(9600);

  for(int pin : touchsPin) {
    pinMode(pin, OUTPUT);
  }

  for(int pin : sensorsBar) {
    pinMode(pin, OUTPUT);
  }

  selectSensorBar(0);
}


void loop() {

  testDetector();

  detectSimple();

  click_classic(0);

  delay(1000);

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

int detectSimple() {
  for(int i = 0; i < 4; i++) {
    if(analogRead(detectorsPin[i]) - blackBlockAcceptable[i + blockAcceptableShift] < detectorTrashold) {
      #if DEBUG
        Serial.print("Detected ");
        Serial.println(i);
      #endif
      return i;
    }
  }
  #if DEBUG
    Serial.println("Error, not detected!");
  #endif
}

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

void testDetector() {
  for(int i = 0; i < 4; i++) {
    Serial.print(analogRead(detectorsPin[i]));
    Serial.print(" | ");
  }
  Serial.println();
}