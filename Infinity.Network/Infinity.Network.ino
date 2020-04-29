#include <SoftwareSerial.h>
#include "EventManager.h"
#include "SerialNetwork.h"
#include "Axe.h"

#define ID 0
#define MODULE_COUNT         1


#define DEBUG                false
#define MASTER               (ID==0)
#define END_PIN              7
#define DIR_PIN              10
#define PUL_PIN              11
#define MASTER_SIZE          250
#define SLAVE_SIZE           160
#define MASTER_TIME          17271
#define SLAVE_TIME           15855
#define MASTER_RAIL_IN_STEP  28000//280000
#define SLAVE_RAIL_IN_STEP   17920//17920//179200
#define RATIO                0.78

const uint32_t vMin = 1000;     // step/sec
const uint32_t vMax = 64000;    // step/sec

#if MASTER
Axe axe(MASTER_RAIL_IN_STEP, true, vMin, vMax);
#else //0.52
Axe axe(SLAVE_RAIL_IN_STEP, false, vMin, vMax, RATIO);
#endif

const uint8_t nodesLen = 1;

const SoftwareSerial nodes [nodesLen] = {
  //SoftwareSerial(A0, A1),
  SoftwareSerial(8, 9)
};

EventManager Manager(ID, MODULE_COUNT);
SerialNetwork Network(nodes, nodesLen);

void setup() {
  delay(3000);
  Serial.begin(9600);
  Network.begin(9600);
  Manager.onGoRun(goRun);
  Manager.onGoHome(goHome);
  Manager.onGoStart(goStart);
  Manager.onStatusChange(statusChange);
  Manager.begin(write);

#if DEBUG
  axe.setDebug(Debug);
  //axe.setDoStep(DoStep);
#else
  axe.setDoStep(DoStep);
  axe.setSetDirection(SetDirection);
#endif
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);
  pinMode(END_PIN, INPUT);

  randomSeed(analogRead(A0) + ID);
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
  float dMax = 1000000.0f / vMin;
    float dMin = 1000000.0f / vMax;
    
    while(digitalRead(END_PIN)){
        digitalWrite(DIR_PIN, Direction);
        DoStep(dMin * 20);
    }
    /*
     while(!digitalRead(END_PIN)){
        SetDirection(HIGH);
        DoStep(dMax);
    }
    return true;
     */
    return false;
}

bool goStart() {
  Serial.println("goStart");
  return random(10.0f) < 1;
}

bool goRun() {
  return random(10.0f) < 1 ;// axe.update();
}

uint32_t t0;
void statusChange(String status) {
  uint32_t t = millis();
  Serial.print(status);
  Serial.print(" : ");
  Serial.println(t - t0);
  t0 = t;
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
