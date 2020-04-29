int totalStepX = 27000;
int totalStepY = 17920;
float batchMaxSizeX = totalStepX/400.0f;
float batchMaxSizeY = totalStepY/400.0f ;

Robot r1, r2;

void setup() {
  size(1000, 800);
  r1 = new Robot(0.2);
  r2 = new Robot(0.1);
  ellipseMode(CENTER);
  background(255);
}
void draw() {
  noStroke();
  translate(width/2, height/2);
  fill(255, 0, 0);

  r1.draw();
  fill(0, 255, 0);
  r2.draw();
}

class Robot {
  Axe x, y;
  Robot(float goTo) {
    x = new Axe(true, totalStepX, goTo);
    y = new Axe(false, totalStepY, goTo);
  }
  void draw() {
    if (x.status == y.status && x.status == STATUS.VIRTUAL_RUNNED) {
      x.status = y.status = STATUS.STARTING;
    } else if (x.status == y.status && x.status == STATUS.WAIT) {
      x.status = y.status = STATUS.RUN;
    } else {
      x.update();
      y.update();
    }
    float pX = x.position /(float)totalStepX * width;
    float pY = y.position /(float)totalStepX * height;


    ellipse(pX, pY, 10, 10);
  }
}
enum STATUS {
  STOP, HELLO, HOMING, AT_HOME, VIRTUAL_RUN, VIRTUAL_RUNNED, STARTING, WAIT, RUN
};
class Axe {
  STATUS status = STATUS.VIRTUAL_RUN;
  int position; 
  int totalStep;
  boolean master;
  int batchMaxSize;
  float alpha, alphaInc;
  float offset;
  float offset2 = 0 ;
  boolean goStart = true;
  int goTo;
  Axe(boolean master, int totalStep, float goTo) {
    this.master = master;
    this.totalStep = totalStep;
    this.batchMaxSize = totalStep/400;
    this.alpha = 0;
    this.alphaInc = (batchMaxSize * 2.f) / (totalStep);
    if (!this.master)this.alphaInc *= 2;
    this.offset = this.master ? 0 : HALF_PI;
    this.position = this.master ? 0 : -height/2;
    this.goTo = (int)(goTo * TWO_PI / alphaInc);
    if (!this.master)this.goTo *= 2;
  }

  void update() {
    switch(status) {
    case VIRTUAL_RUN:
      if (virtualRun())status = STATUS.VIRTUAL_RUNNED;
      break;
    case STARTING:
      if (goStart())status = STATUS.WAIT;
      break ;
    case RUN:
      if (run())status = STATUS.STARTING;
      break;
    default:
      break;
    }
  }

  boolean goStart() {
    for (int i = 0; i < 10 && position != 0; i ++) {
      position += (position > 0 ? -1 : 1);
    }
    return position == 0 ;
  }
  boolean run() {
    float sX = sin(alpha + offset);
    boolean  dX = sX > 0 ;
    int batchSizeX = abs(round(sX * batchMaxSize));
    for (int i = 0; i < batchSizeX; i ++ ) {
      position += dX ? 1 : -1;
    }
    alpha += alphaInc;
    if (alpha >= (master ? TWO_PI : 2*TWO_PI)) {
      alpha = 0;
      return true;
    }
    return false;
  }
  boolean virtualRun() {
    if (this.goTo > 0) {
      float sX = sin(alpha + offset);
      boolean  dX = sX > 0 ;
      int batchSizeX = abs(round(sX * batchMaxSize));
      for (int i = 0; i < batchSizeX; i ++ ) {
        position += dX ? -1 : 1;
      }
      alpha += alphaInc;
      if (alpha  >= (master ? TWO_PI : 2*TWO_PI)) {
        alpha = 0;
      }
      this.goTo --;
    }
    if (this.goTo <= 0) {
      offset += alpha;
      alpha = 0 ;
      return true;
    }
    return false;
  }
}
