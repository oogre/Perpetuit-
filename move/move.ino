#define Y_DIR  8
#define Y_PUL  9
#define X_DIR 10
#define X_PUL 11


void setup() {
  // put your setup code here, to run once:
  pinMode(X_DIR, OUTPUT);
  pinMode(X_PUL, OUTPUT);

  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_PUL, OUTPUT);

  float speedRatio = 1;
  for (int i = 0 ; i < 6400 ; i ++) {
    move(X_PUL, X_DIR, LOW, speedRatio);
    move(Y_PUL, Y_DIR, LOW, 1 - speedRatio);
    delayMicroseconds(100);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

void move(int pulsePin, int dirPin, bool direction, float speedRatio) {
  digitalWrite(dirPin, direction);
  digitalWrite(pulsePin, HIGH);
  delayMicroseconds(20 + (500 * speedRatio));
  digitalWrite(pulsePin, LOW);
  delayMicroseconds(20 + (500 * speedRatio));
}
