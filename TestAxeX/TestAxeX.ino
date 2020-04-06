#define X_DIR 10
#define X_STE 11

int state = 0;
long nextStatAt = 0;
int stateDuration = 1000;



bool DIR = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STE, OUTPUT);
}

void loop() {
  if (state == 0) {
    if (millis() > nextStatAt) {
      DIR = !DIR;
      setDirection(DIR);
      nextStatAt = millis() + stateDuration;
      state = 1;
      
    }
  }
  else if (state == 1) {
    digitalWrite(X_STE, HIGH);
    delayMicroseconds(20);
    digitalWrite(X_STE, LOW);
    delayMicroseconds(20);
    if (millis() > nextStatAt) {
      nextStatAt = millis() + stateDuration;
      state = 0;
    }
  }
}

void setDirection(bool dir){
  digitalWrite(X_DIR, !dir);
}
