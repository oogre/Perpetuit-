#include <math.h>
#define MASTER_PIN 9
#define SLAVE_PIN  2
#define LED_PIN    6

#define MASTER true
#define SLAVE  !MASTER

unsigned long master_click = 0;
unsigned long slave_click = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(MASTER_PIN, OUTPUT);
  pinMode(SLAVE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SLAVE_PIN), blink, CHANGE);
  //Serial.begin(9600);
}

void loop() {
  if (MASTER) {
    master_click ++;
    digitalWrite(MASTER_PIN, !digitalRead(MASTER_PIN));
  }
  unsigned long click = (MASTER ? master_click : slave_click);
  float alpha = (float)fmod(click * 0.001f,  TWO_PI);
  float value = (cos(alpha) + 1) * 0.5f;
  float v = value * 255;
  analogWrite(LED_PIN, v);
}

void blink() {
  slave_click ++;
}
