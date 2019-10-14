#define detectorPin 5
#define touchPin 6

int detectorValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(detectorPin, INPUT);
  pinMode(touchPin, OUTPUT);
}

void loop() {
  Serial.println("mleko");
  delay(1000);
  detectorValue = analogRead(detectorPin);
  Serial.println(detectorValue);
}
