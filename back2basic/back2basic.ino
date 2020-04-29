#include "iSin.h"

#define DEBUG           false
#define MASTER          true
#define DIR_PIN           10
#define PUL_PIN           11
#define MASTER_SIZE 250
#define SLAVE_SIZE 160
#define MASTER_TIME     25293
#define SLAVE_TIME      25638
#define MASTER_RAIL_IN_STEP  1000//280000
#define SLAVE_RAIL_IN_STEP   28000//17920//179200
#define RATIO                1

bool directionX = HIGH;
bool directionY = HIGH;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

float stepCx = 0 ;
float stepCy = 0 ;


void loop() {
  float rx = stepCx / (float)MASTER_RAIL_IN_STEP;
  float v = isin(rx * 360) * 50;
  if(v < 1)stepCx += 0.001;
  for (int i = 0 ; i < v ; i ++) {
    stepCx += directionX ? 1 : -1;
    if (stepCx >= MASTER_RAIL_IN_STEP || stepCx <= 0) {
      directionX = !directionX;
      break;
    }
  }
  Serial.println(stepCx);
}
