//1410 MASTER
//1420 SLAVE

#define DEBUG           false
#define MASTER          false
#define ENABLE_PIN         2
#define ALIVE_PIN          3
#define DIR_PIN           10
#define PUL_PIN           11

#define MASTER_RAIL_IN_STEP  290000
#define SLAVE_RAIL_IN_STEP   200000
#if MASTER
#define RAIL_IN_STEP  MASTER_RAIL_IN_STEP
#else
#define RAIL_IN_STEP  SLAVE_RAIL_IN_STEP
#endif
#define SIN_RES          360
const uint32_t amplitude = 2 * RAIL_IN_STEP;

#if MASTER
const uint16_t stepPerDegree = amplitude / (float)SIN_RES ;
const float degreePerStep = 1.0f / stepPerDegree;
#else
const uint16_t stepPerDegree = 2 * amplitude / (float)SIN_RES ;
const float degreePerStep = 1.0f / stepPerDegree;
#endif

void setup() {
#if DEBUG
  Serial.begin(9600);
#endif
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(DIR_PIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);
}

void loop() {
  for (uint32_t i = 0 ; i < amplitude ; i++) {
#if MASTER
    uint16_t alpha = i * degreePerStep;
#else
    uint16_t alpha = i * degreePerStep;// + 270;
    if (alpha >= 360) alpha %= 360;
#endif
    float del = abs(isin(alpha + 90));
    uint8_t dir = (alpha >= 0 && alpha <= 180);
    uint16_t _delay = (uint16_t) lerp(20, 200, del * del * del * del * del * del * del * del * del * del);
#if DEBUG
    Serial.print(dir * 40);
    Serial.print(" ");
    Serial.println(_delay);
#else
    digitalWrite(DIR_PIN, dir);
    for (uint16_t s = 0 ; s < stepPerDegree ; s++) {
      digitalWrite(PUL_PIN, HIGH);
      delayMicroseconds(1);
      digitalWrite(PUL_PIN, LOW);
      delayMicroseconds(_delay);
    }
#endif
    i += stepPerDegree;
  }
}

float lerp(float a, float b, float t) {
  return (a + t * (b - a) );
}
