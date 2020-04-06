#define DEBUG          false
#define MASTER          true
#define ENABLE_PIN         2
#define ALIVE_PIN          3
#define DIR_PIN           10
#define PUL_PIN           11

#define MASTER_RAIL_IN_STEP  36800
#define SLAVE_RAIL_IN_STEP   20000


const uint16_t ONE_QUARTER_PI   =  HALF_PI * 0.5;
const float vMin = 0.1;    // step/millisec
const float vMax = 1000;   // step/millissec
const uint8_t len = 90;
uint16_t value [90];
class Axe {
    float ratio;
    uint32_t totalStep;
    float totalDuration;
    uint16_t maxDuration = 0 ;
    uint16_t minDuration = pow(2, 16) ;
    
  public :
    Axe(uint32_t totalStep, float ratio = 1.0f)
    {
      this->totalStep = totalStep;
      this->ratio = ratio;
      this->maxDuration = (int)(1000.0f / (vMin * this->ratio ));
      this->minDuration = (int)(1000.0f / (vMax * this->ratio ));
    };
    void update() {
      for (uint32_t p = 0; p < totalStep; p ++) {
        float alpha = TWO_PI * p / (float)totalStep;
#if MASTER
        float v   = (cos(2 * alpha + PI) + 1) * 0.5;
        uint8_t d = alpha >= 0 && alpha <= PI;
#else
        float v   = (cos(4 * alpha) + 1) * 0.5;
        uint8_t d = (alpha >= 0                  && alpha <= ONE_QUARTER_PI ) ||
                    (alpha >= 3 * ONE_QUARTER_PI && alpha <= 5 * ONE_QUARTER_PI) ||
                    (alpha >= 7 * ONE_QUARTER_PI && alpha <= TWO_PI);
#endif
        v = lerp(vMin * ratio, vMax * ratio, v);
        float stepDuration = (float)(1.0f / v);
        totalDuration += stepDuration;
        //Serial.println(stepDuration);
      }
      Serial.print(value[0]);
      Serial.print("Steps : ");
      Serial.print(totalStep);
      Serial.print("\tmaxDuration : ");
      Serial.print(maxDuration);
      Serial.print("\tminDuration : ");
      Serial.print(minDuration);
      Serial.print("\tTime : ");
      Serial.println(totalDuration);
    }
  private :
    float lerp(float a, float b, float t) {
      return (a + t * (b - a) );
    }
};

void setup() {
  for (int i = 0 ; i < len; i ++) {
    value[i] = random(100);
  }
  Serial.begin(9600);
  Axe x(MASTER_RAIL_IN_STEP);
  x.update();
  Axe y(SLAVE_RAIL_IN_STEP, SLAVE_RAIL_IN_STEP / (float)MASTER_RAIL_IN_STEP);
  y.update();
}

void loop() {


}
