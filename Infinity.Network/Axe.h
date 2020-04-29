#include "iSin.h"

class Axe {
  public :
    bool master;
    uint32_t cuspPoint; // where the direction reverse // point de rebroussement
    uint32_t step;
    uint32_t tStep;
    uint32_t rStep;
    uint32_t stepPerUnit;
    long counter = 0 ;
    float ratio;
    float vMin;
    float vMax;
    uint16_t offset;
    uint32_t beta;      // the animation paramater [0, betaMax]
    uint16_t cycle;
    uint32_t betaMax;   // 720 (master) || 1440 (!master)
    bool isFirstMove = true;
    float stepDuration;
    uint8_t direction = false; // it's inverted at first move;
    float m = 0.05;
    float mX;
    float mY;
    float _mY;
    
    void (*doStep)(float);
    void (*setDirection)(uint8_t);
    void (*debug)(uint16_t, uint16_t);
  public :
    Axe(uint32_t totalStep, bool master, uint32_t vMin, uint32_t vMax, float ratio = 1.0f)
      : beta(0),
        step(0),
        tStep(0),
        vMax(1000000.f / vMin),
        vMin(1000000.f / vMax),
        ratio(ratio),
        master(master),
        offset(master ? 0 : 180),
        stepDuration(1000000.0f / vMin)
    {
      this->cycle = master ? 2 : 4;
      this->stepPerUnit = totalStep / 360;
      this->cuspPoint = this->stepPerUnit * 360;
      this->betaMax = this->cuspPoint * this->cycle / this->stepPerUnit;
      this->rStep = master ? 0 : (this->cuspPoint >> 1) ;
      m = 0.08;
      mX = lerp(0, 1, m);
      mY = lerp(1, 0, m);
      _mY = 1.0f / mY;
    };

    bool update() {
      if (rStep == 0) {
        direction = !direction;
      }
      if (step == 0) {
        float _v_ = isin(beta + offset);
        float t = lerp(mX, mY, _v_);
        t = 1.f / t;
        t = (t - _mY) * (mY * mX) / (mY - mX);
        float d = lerp(vMin, vMax, t) * ratio;
        /*
          float _v_ = isin(beta + offset);
          float v = lerp(vMin, vMax, _v_) * ratio;
          float d = 1000000.0f / v;
        */
        if (!master && isFirstMove) {
          stepDuration += (d - stepDuration) * 0.1;
        } else {
          stepDuration = d;
        }

        if (debug)(*debug)(stepDuration, direction * 500);
        beta ++;
      }

      if (setDirection)(*setDirection)(direction);
      if (doStep)(*doStep)(stepDuration);
      counter += (direction) ? 1 : -1;
      tStep++;
      rStep++;
      step ++;

      if (rStep >= cuspPoint) {
        rStep = 0 ;
        isFirstMove = false;
      }

      if (step >= stepPerUnit) {
        step = 0 ;
      }

      if (beta >= betaMax && counter == 0 ) {//tStep >?= endPoint
        beta = 0;
        step = 0;
        tStep = 0 ;
        return true;
      }
      return false;
    }

    void setDoStep(void (*doStep)(float)) {
      this->doStep = doStep;
    }

    void setSetDirection(void (*setDirection)(uint8_t)) {
      this->setDirection = setDirection;
    }

    void setDebug(void (*debug)(uint16_t, uint16_t)) {
      this->debug = debug;
    }

    float lerp(float a, float b, float t) {
      return (a + t * (b - a) );
    }
};
