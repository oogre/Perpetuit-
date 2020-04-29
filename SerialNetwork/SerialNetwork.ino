#include <SoftwareSerial.h>
#include "EventManager.h"
#include "SerialNetwork.h"
#include "Axe.h"

#define ID 1
#define MODULE_COUNT 2

#define MASTER                false
#define DIR_PIN               10
#define PUL_PIN               11
#define MASTER_SIZE           250
#define SLAVE_SIZE            160
#define MASTER_TIME           75959
#define SLAVE_TIME            75656
#define MASTER_RAIL_IN_STEP   74000
#define SLAVE_RAIL_IN_STEP    47360 * 1.5

const float vMin = 0.1;    // step/millisec
const float vMax = 10;   // step/millissec

const uint8_t nodesLen = 1;

const SoftwareSerial nodes [nodesLen] = {
  //SoftwareSerial(A0, A1),
  SoftwareSerial(8, 9)
};

EventManager Manager(ID, MODULE_COUNT);
SerialNetwork Network(nodes, nodesLen);

#if MASTER
Axe axe(MASTER_RAIL_IN_STEP, true, vMin, vMax);
#else //0.52
Axe axe(SLAVE_RAIL_IN_STEP, false, vMin, vMax, 1.15 * SLAVE_RAIL_IN_STEP / (float)MASTER_RAIL_IN_STEP);
#endif

void setup() {
  Serial.begin(9600);
  Network.begin(9600);

  Manager.onGoRun(goRun);
  Manager.onGoHome(goHome);
  Manager.onGoStart(goStart);
  Manager.begin(write);

  randomSeed(analogRead(A0) + ID);

  pinMode(DIR_PIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);
}

void loop() {
  uint8_t inByte = 0xF0;
  if (Manager.isInputNeeded) {
    inByte = Network.run();
  }
  Manager.run(inByte);
}


void write(uint8_t outByte) {
  Network.write(outByte);
}

bool goHome() {
  Serial.println("goHome");
  return random(10.0f) < 1;
}

bool goStart() {
  Serial.println("goStart");
  return random(10.0f) < 1;
}

bool goRun() {
  Serial.println("goRun");
  axe.update(DoStep, SetDirection);
  return true;
  //return random(10.0f) < 1;
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
  Duration = firstMove ? 500 : Duration;
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
