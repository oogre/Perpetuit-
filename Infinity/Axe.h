#include "iSin.h"

class Axe {
    float dvM;
    float vMin;
    bool master;
    uint16_t totalStep;
    float anglePerStep;
    uint16_t frq = this->master ? 1 :  2;
    uint16_t off = this->master ? 0 : 90; // start angle
  public :
    Axe(uint16_t totalStep, bool master, float vMin, float vMax, float ratio = 1.0f)
    {
      this->master = master;
      this->frq = this->master ? 1 :  2;
      this->off = this->master ? 0 : 90; // start angle
      this->totalStep = totalStep;
      this->anglePerStep = 2 * 360.0f / totalStep;
      this->vMin = vMin * ratio;
      this->dvM = vMax * ratio - this->vMin;
    };
    
    void update() {
      float alpha = 0;
      uint16_t beta = alpha;
      for (uint16_t p = 0 ; p < totalStep; ) {
        uint16_t beta = (uint16_t)alpha;
        float v   = isin(frq * (alpha + off));
        uint8_t d = (this->master && alpha >= 0 && alpha <= 360) ||
                    (!this->master &&
                     (
                       alpha <= 90 ||
                       (alpha >= 270 && alpha <= 450) ||
                       alpha >= 630
                     )
                    );
        v = vMin + v * dvM; // linear interpolation
        float stepDuration = 1000.0 / v;
        while (beta == (uint16_t)alpha && p < totalStep) {
          // DO_STEP;
          alpha += anglePerStep;
          p ++;
        }
      }
    }
};
