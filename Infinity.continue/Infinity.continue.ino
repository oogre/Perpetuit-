#include "Axe.h"
#define DEBUG           false
#define MASTER          true
#define DIR_PIN           10
#define PUL_PIN           11
#define MASTER_SIZE 250
#define SLAVE_SIZE 160
#define MASTER_TIME     25293
#define SLAVE_TIME      25638
#define MASTER_RAIL_IN_STEP  28000//280000
#define SLAVE_RAIL_IN_STEP   28000//17920//179200
#define RATIO                1

const uint32_t vMin = 3200;     // step/sec
const uint32_t vMax = 64000;    // step/sec

#if MASTER
Axe axe(MASTER_RAIL_IN_STEP, true, vMin, vMax);
#else 
Axe axe(SLAVE_RAIL_IN_STEP, false, vMin, vMax,  RATIO);
#endif


void setup() {
  Serial.begin(9600);

#if DEBUG
  axe.setDebug(Debug);
  //axe.setDoStep(DoStep);
#else
  axe.setDoStep(DoStep);
  axe.setSetDirection(SetDirection);
#endif

  pinMode(DIR_PIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);

  delay(3000);
}
uint32_t t0;
void loop() {
  if (axe.update()) {
    uint32_t t = millis();
    //Serial.println(t - t0);
    t0 = t;
  }
}

void SetDirection(uint8_t Direction) {
  digitalWrite(DIR_PIN, Direction);
}

void DoStep(float Duration) {
  digitalWrite(PUL_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(PUL_PIN, LOW);
  if (Duration > 16383) {
    delay(Duration * 0.001);
  } else {
    delayMicroseconds(Duration);
  }
}

void Debug(uint16_t Delay, uint16_t Direction) {
  Serial.print(Delay);
  Serial.print(", ");
  Serial.println(Direction);
}
