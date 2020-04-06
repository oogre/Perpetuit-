#include<SPI.h>

#define MASTER     false
#define LED_PIN    6

#if MASTER
bool strob = false;
#endif

uint16_t period = 1000; // in click
uint16_t phase = period * TWO_PI;
float frequency = 1.0f / period;
uint16_t click = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

#if MASTER
  {
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV8);
    digitalWrite(SS, HIGH);
  }
#else
  { pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);
    SPI.attachInterrupt();
  }
#endif
}


void loop() {
#if MASTER
  {
    digitalWrite(SS, LOW);
    strob = SPI.transfer(strob);
    click ++;
    click %= phase;
  }
#endif
  move();
}

#if !MASTER

ISR (SPI_STC_vect)
{
  SPDR = !SPDR;
  click ++;
  click %= phase;
}

#endif


void move() {
  float alpha = click * frequency;
#if MASTER
  {
    alpha = sin(alpha);
  }
#else
  {
    alpha = cos(alpha * 2 - PI * 0.5);
  }
#endif
  alpha = (alpha + 1) * 127;
  analogWrite(LED_PIN, alpha);
  Serial.write((byte)alpha);
}
