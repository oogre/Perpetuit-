//1410 MASTER
//1420 SLAVE


#define MASTER       true
#define ENABLE_PIN   2
#define ALIVE_PIN    3
#define DIR_PIN     10
#define PUL_PIN     11

uint16_t amplitude = 4000; // in click
uint16_t period = 1000; // in click
uint16_t phase = period * TWO_PI;
float frequency = 1.0f / period;

volatile uint16_t click = 0;
volatile uint32_t time = 0;
volatile uint32_t blastedAt = 0;
volatile uint32_t LastStepAt = 0;
volatile bool blasted = false;
uint16_t step_counter = 3200;
uint16_t oldPosition = 0;
uint16_t max_step = 0 ;
void setup() {

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ALIVE_PIN, OUTPUT);
  pinMode(ENABLE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENABLE_PIN), BLAST, CHANGE);
  
  for (click = 0 ; click < phase ; click++) {
    int position = sin(click * frequency) * amplitude;
    int steps = oldPosition - position;
    oldPosition = position;
    max_step = max(max_step, abs(steps));
  }
  click = 0;
  
  while (MASTER && !blasted) {
    delay(200);
    digitalWrite(ALIVE_PIN, !digitalRead(ALIVE_PIN));
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(ALIVE_PIN, !digitalRead(ALIVE_PIN));
    digitalWrite(LED_BUILTIN, LOW);
  }

  pinMode(DIR_PIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);

  
}

void loop() {
  time = millis();

  if ((time - blastedAt) > 100) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (blasted) {
    blasted = false;

    //if (step_counter > 0) {

    int position = sin(click * frequency) * amplitude;
    int steps = oldPosition - position;
    oldPosition = position;

    //Serial.println(steps);
    //Serial.print(", ");
    //Serial.println(steps);
    //Serial.print(" ");
    //Serial.println("");

    digitalWrite(DIR_PIN, steps >= 0 ? HIGH : LOW);
    for (int i = 0 ; i < abs(steps); i ++) {
      digitalWrite(PUL_PIN, HIGH);
      delayMicroseconds(1);
      digitalWrite(PUL_PIN, LOW);
      delayMicroseconds(1);
    }

    //delayMicroseconds(max_step - abs(steps));

    // step_counter --;
    // }
    digitalWrite(ALIVE_PIN, !digitalRead(ALIVE_PIN));
  }
}

void BLAST ()
{
  click ++;
  click %= phase;
  blasted = true;
  blastedAt = time;
}
