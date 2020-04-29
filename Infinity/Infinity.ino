#include "Axe.h"

#define MASTER          false
#define DIR_PIN           10
#define PUL_PIN           11
#define MASTER_SIZE 250
#define SLAVE_SIZE 160
#define MASTER_TIME 75959
#define SLAVE_TIME 75656
#define MASTER_RAIL_IN_STEP  74000
#define SLAVE_RAIL_IN_STEP   47360 * 1.5

const float vMin = 0.1;    // step/millisec
const float vMax = 10;   // step/millissec

#if MASTER
Axe axe(MASTER_RAIL_IN_STEP, true, vMin, vMax);
#else //0.52
Axe axe(SLAVE_RAIL_IN_STEP, false, vMin, vMax, SLAVE_RAIL_IN_STEP / (float)MASTER_RAIL_IN_STEP);
#endif


void setup() {
  Serial.begin(9600);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);
}

void loop() {
  uint32_t t0 = millis();
  axe.update(DoStep, SetDirection);
  Serial.println(millis() - t0);
}


void SetDirection(uint8_t Direction) {
  digitalWrite(DIR_PIN, Direction);
}

uint16_t maxDuration = 0;
bool firstMove = !MASTER;

void DoStep(uint16_t Duration) {

#if MASTER
#else
  if (firstMove) {
    if (Duration >= maxDuration) {
      maxDuration = Duration;
    } else {
      firstMove = false;
    }
  }
#endif
  digitalWrite(PUL_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(PUL_PIN, LOW);
  Duration = firstMove ? 1000 : Duration;
  if (Duration > 16383) {
    delay(Duration * 0.001);
  } else {
    delayMicroseconds(Duration);
  }
}

void Debug(uint16_t Delay, uint8_t Direction) {
  Serial.print(Delay);
  Serial.print(", ");
  Serial.println(Direction * 100);
}
