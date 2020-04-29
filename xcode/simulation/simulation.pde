boolean master = false;
int totalStepX = 27000;
int totalStepY = 17920;
float resolution = 1.0f/400;
float batchMaxSizeX = totalStepX * resolution;
float batchMaxSizeY = totalStepY * resolution ;
PGraphics pg;
Robot r1, r2;

void setup() {

  r1 = new Robot(-height/2);
  r2 = new Robot(height/2);
  r1.goTo(0);
  r2.goTo(0.33);
  size(1000, 640, P2D);

  pg = createGraphics(width, height, P2D);
  pg.beginDraw();
  pg.background(255);
  pg.endDraw();

  noStroke();
}

void draw() {
  
  background(255);
  image(pg, 0, 0);
  translate(0, height/2);
  pg.beginDraw();
  pg.translate(0, height/2);
  pg.noStroke();
  pg.fill(0);
  r1.draw(10);
  pg.fill(255);
  r2.draw(20);
  pg.endDraw();
}

class Robot {
  long x = 0 ;
  long y = height/2;
  float alpha = 0;
  float beta = 0;
  float alphaInc;
  float betaInc;
  float d;
  boolean goStart = true;
  Robot(float d) {
    this.d = d;
    alphaInc = 2.f * resolution ; 
    betaInc = 2.f * 2.f * resolution ;
    println(alphaInc);
    println(betaInc);
  }

  void goTo(float ratio) {
    int a = (int)(ratio * TWO_PI / alphaInc);
    while (a-->0) {
      draw(0);
    }
  }

  void draw(int size) {
    if (goStart) {
      if (x != 0) {
        x += x>0 ? -1 : 1;
      }
      if (y != 0) {
        y += y > 0 ? -1 : 1;
      }
      if (x==0 && y==0) {
        goStart=false;
      }
    } else {
      float sX = sin(alpha);
      float sY = sin(beta+HALF_PI);
      boolean  dX = sX > 0 ;
      boolean  dY = sY > 0 ;
      int batchSizeX = abs(round(sX * batchMaxSizeX));
      int batchSizeY = abs(round(sY * batchMaxSizeY));
      for (int i = 0; i < max(batchSizeX, batchSizeY); i ++ ) {
        if (i < batchSizeX)x += dX ? 1 : -1;
        if (i < batchSizeY)y += dY ? 1 : -1;
      }
      alpha += alphaInc;
      beta += betaInc;
      if (alpha  >= TWO_PI || beta >= (2 * TWO_PI)) {
        alpha = 0;
        beta = 0;
      }
    }
    if (size != 0 ) {
      pg.ellipse((x/(float)totalStepX) * width, (y/(float)totalStepY) * height, size, size);
    }
    stroke(0);
    line((x/(float)totalStepX) * width, (y/(float)totalStepY) * height, (x/(float)totalStepX) * width, d);
    noStroke();
  }
}
