#define X_DIR 10
#define X_STE 11

int PPR = 6400;
unsigned long X_LENGTH = 617600; 

unsigned long step = 305600;

void setup() {
  // put your setup code here, to run once:
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STE, OUTPUT);
  
  setDirection(LOW);
  
  unsigned long stepCounter = 0 ;
  while (stepCounter < step) {
    digitalWrite(X_STE, HIGH);
    delayMicroseconds(40);
    digitalWrite(X_STE, LOW);
    delayMicroseconds(40);
    stepCounter ++;
  }
  
}

void loop() {

}

void setDirection(bool dir) {
  digitalWrite(X_DIR, dir);
}
