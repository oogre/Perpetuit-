#include <AccelStepper.h>
#define Y_DIR     8
#define Y_PUL     9
#define X_DIR    10
#define X_PUL    11

#define WIDTH  2250
#define HEIGHT 1250

//#define MIN(A, B)     (((A) < (B)) ? (A) : (B))

float cycle_duration = 160.0f; //Seconds
float cycle_Frq = 0.001f/cycle_duration;

float ratio = HEIGHT / (float) WIDTH;

void doStep(int pulsePin) {
  digitalWrite(pulsePin, HIGH);
  delayMicroseconds(1);
  digitalWrite(pulsePin, LOW);
  delayMicroseconds(1);
}
void forwardX() {
  digitalWrite(X_DIR, HIGH);
  //doStep(X_PUL);
  digitalWrite(X_PUL, HIGH);
  delayMicroseconds(1);
  digitalWrite(X_PUL, LOW);
  delayMicroseconds(1);
}
void backwardX() {
  digitalWrite(X_DIR, LOW);
  //doStep(X_PUL);
  digitalWrite(X_PUL, HIGH);
  delayMicroseconds(1);
  digitalWrite(X_PUL, LOW);
  delayMicroseconds(1);
}

void forwardY() {
  digitalWrite(Y_DIR, HIGH);
  //doStep(Y_PUL);
  digitalWrite(Y_PUL, HIGH);
  delayMicroseconds(1);
  digitalWrite(Y_PUL, LOW);
  delayMicroseconds(1);
}
void backwardY() {
  digitalWrite(Y_DIR, LOW);
  //doStep(Y_PUL);
  digitalWrite(Y_PUL, HIGH);
  delayMicroseconds(1);
  digitalWrite(Y_PUL, LOW);
  delayMicroseconds(1);
}

AccelStepper stepperX(forwardX, backwardX);
AccelStepper stepperY(forwardY, backwardY);

void setup() {
  // put your setup code here, to run once:
  pinMode(X_DIR, OUTPUT);
  pinMode(X_PUL, OUTPUT);

  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_PUL, OUTPUT);
  stepperX.setSpeed(0);
  stepperY.setSpeed(0);
  
}

void loop() {
  float alpha = millis() * cycle_Frq * TWO_PI;
  
  stepperX.setSpeed(cos(alpha) * 1000);
  stepperY.setSpeed(cos(2 * alpha + PI) * 1000 * ratio);
  stepperX.runSpeed();
  stepperY.runSpeed();
}
