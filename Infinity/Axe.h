#include "iSin.h"

const float ONE_QUARTER_PI   =  HALF_PI * 0.5f;
const float THREE_QUARTER_PI =  3 * ONE_QUARTER_PI;
const float FIVE_QUARTER_PI  =  5 * ONE_QUARTER_PI;
const float SEVEN_QUARTER_PI =  7 * ONE_QUARTER_PI;

const float vMin = 1;    // step/millisec
const float vMax = 1000;   // step/millissec

class Axe {
    uint32_t totalStep;
    float radianPerStep;
    float degreePerStep;
    uint32_t totalDuration;
    float _vMin;
    float _vMax;
    float _dvM;
    bool master;
  public :
    Axe(uint32_t totalStep, bool master, float ratio = 1.0f)
    {
      this->master = master;
      this->totalStep = totalStep;
      //this->radianPerStep = 2 * TWO_PI/totalStep;
      this->degreePerStep = 2 * 360.0f / totalStep;
      _vMin = vMin * ratio;
      _vMax = vMax * ratio;
      _dvM = _vMax - _vMin;
    };
    void update() {
      float alpha = 0;
      uint32_t beta = alpha;
      int frq = this->master ? 1 :  2;
      int off = this->master ? 0 : 90; // start angle
      for (uint32_t p = 0 ; p < totalStep; ) {
        uint32_t beta = (uint32_t)alpha;
        float v   = isin(frq * (alpha + off));
        uint8_t d = (this->master && alpha >= 0 && alpha <= 360) ||
                    (!this->master &&
                     (
                       alpha <= 90 ||
                       (alpha >= 270 && alpha <= 450) ||
                       alpha >= 630
                     )
                    );
        v = _vMin + v * _dvM; // linear interpolation
        float stepDuration = 1000.0 / v;
        while (beta == (uint32_t)alpha && p < totalStep) {
          // DO_STEP;
          totalDuration += stepDuration;
          alpha += degreePerStep;
          p ++;
        }
      }
    }
};
