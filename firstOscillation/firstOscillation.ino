#define X_DIR 10
#define X_STE 11

unsigned int PPR = 6400;
unsigned long X_LENGTH = 617600;

float cycle_duration = 100.0f; //Seconds
float cycle_Frq = 0.001f/cycle_duration;

void setup() {
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STE, OUTPUT);

  //Serial.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:
  float alpha = millis() * cycle_Frq * TWO_PI;
  float speed = cos(alpha);
  int _delay = (int) lerp(5, 500, pow(1-abs(speed), 1));
  if (false) {
    Serial.print(speed* 500);
    Serial.print(", ");
    Serial.println(_delay);
  } else {
    setDirection(speed > 0);
    digitalWrite(X_STE, HIGH);
    delayMicroseconds(_delay);
    digitalWrite(X_STE, LOW);
    delayMicroseconds(_delay);
  }
}

float lerp(float a, float b, float t) {
  return (a + t * (b - a) );
}

void setDirection(bool dir) {
  digitalWrite(X_DIR, dir);
}
