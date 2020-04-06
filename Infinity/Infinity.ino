#include "Axe.h"

#define MASTER          true
#define DEBUG          false
#define ENABLE_PIN         2
#define ALIVE_PIN          3
#define DIR_PIN           10
#define PUL_PIN           11

#define MASTER_RAIL_IN_STEP  36800
#define SLAVE_RAIL_IN_STEP   20000

const float vMin = 1;    // step/millisec
const float vMax = 1000;   // step/millissec

#if MASTER
Axe axe(MASTER_RAIL_IN_STEP, MASTER, vMin, vMax);
#else
Axe axe(SLAVE_RAIL_IN_STEP, !MASTER, vMin, vMax, SLAVE_RAIL_IN_STEP / (float)MASTER_RAIL_IN_STEP);
#endif

void setup() {
#if DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  axe.update();
}
