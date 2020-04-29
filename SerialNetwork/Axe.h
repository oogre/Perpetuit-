#include "iSin.h"

class Axe {
    float dvM;
    float vMin;
    bool master;
    uint32_t totalStep;
    float anglePerStep;
    uint16_t frq = this->master ? 1 :  2;
    uint16_t off = this->master ? 0 : 90; // start angle
  public :
    Axe(uint32_t totalStep, bool master, float vMin, float vMax, float ratio = 1.0f)
    {
      this->master = master;
      this->frq = this->master ? 1 :  2;
      this->off = this->master ? 0 : 90; // start angle
      this->totalStep = totalStep;
      this->anglePerStep = 2 * 360.0f / totalStep;
      this->vMin = vMin * ratio;
      this->dvM = vMax * ratio - this->vMin;
    };

    void update(void (*doStep)(uint16_t) = Axe::doStep, void (*setDirection)(uint8_t) = Axe::setDirection, void (*debug)(uint16_t, uint8_t) = Axe::debug) {
      float alpha = 0;
      uint16_t beta = alpha;
      for (uint32_t p = 0 ; p < totalStep; ) {
        uint16_t beta = (uint16_t)alpha;
        float v   = isin(frq * (alpha + off));
        uint8_t direction = (this->master && alpha >= 0 && alpha <= 360) ||
                            (!this->master &&
                             (
                               alpha <= 90 ||
                               (alpha >= 270 && alpha <= 450) ||
                               alpha >= 630
                             )
                            );
        v = vMin + v * dvM; // linear interpolation
        float stepDuration = 1000.0 / v;
        (*setDirection)(direction);
        (*debug)(stepDuration, direction);
        while (beta == (uint16_t)alpha && p < totalStep) {
          (*doStep)(stepDuration);
          alpha += anglePerStep;
          p ++;
        }
      }
    }

    static void doStep(uint16_t delay) {}
    static void setDirection(uint16_t direction) {}
    static void debug(uint16_t delay, uint16_t direction) {}
};
