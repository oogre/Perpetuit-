#define DIR_PIN              10
#define PUL_PIN              11


void setup() {
  // put your setup code here, to run once:
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float a = (1-((1-cos(millis() * 0.001))*0.5));
  float d = 100 + a * 300;//map (, 0.0f, 1.0f, 1000, 100);
  
  digitalWrite(PUL_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(PUL_PIN, LOW);
  delayMicroseconds(d);
}
