final int X_RAIL_SIZE =  360000;
final int Y_RAIL_SIZE =  200000;
//final double QUARTER_PI =  HALF_PI * 0.5;
final double THREE_QUARTER_PI =  3 * QUARTER_PI;
final double FIVE_QUARTER_PI =  5 * QUARTER_PI;
final double SEVEN_QUARTER_PI =  7 * QUARTER_PI;
float vMin = 10;    // step/millisec
float vMax = 100;   // step/millissec

void setup() {
  size(360, 360);
  background(255);
  Axe x = new Axe(new MoveProcessX(), new Drawer(color(255, 0, 0)), X_RAIL_SIZE);
  println(" ");
  Axe y = new Axe(new MoveProcessY(), new Drawer(color(0, 0, 255)), Y_RAIL_SIZE, Y_RAIL_SIZE/(float)X_RAIL_SIZE);
  blendMode(MULTIPLY);
  x.draw();
  y.draw();
}


class Axe {
  int totalStep;
  MoveProcess sp;
  public Drawer dr;
  int totalDuration = 0 ;
  Axe(MoveProcess sp, Drawer dr, int totalStep) {
    this(sp, dr, totalStep, 1);
  }
  Axe(MoveProcess sp, Drawer dr, int totalStep, float ratio) {
    this.totalStep = totalStep;
    this.sp = sp;
    this.dr = dr;

    for (int p = 0; p < totalStep; p ++) {
      float alpha = TWO_PI * p/(float)totalStep;
      int stepDuration = 0;
      float v = sp.getSpeed(alpha);
      int d = sp.getDirection(alpha);
      v = lerp(vMin*ratio, vMax*ratio, v);  
      stepDuration = (int)(1000 / v);

      dr.drawDuration(p / (float)totalStep, map(stepDuration, 1000.0f/(vMin*ratio), 1000.0f/(vMax*ratio), 0, 1));
      dr.drawDirection(p / (float)totalStep, d);
      totalDuration += stepDuration;
      
     // println(stepDuration);
    }

    println("TOTAL : " + (totalDuration/1000000.0));
  }
  public void draw() {
    dr.draw();
  }
}

public class MoveProcessX implements MoveProcess 
{
  public float getSpeed(float alpha) 
  {
    //return (icos((int)(RAD_TO_DEG * 2 * alpha + PI))+1)*0.5;
    return (cos(2 * alpha + PI)+1)*0.5;
  }
  public int getDirection(float alpha) {
    return (alpha >= 0 && alpha <= PI) ? 1 : 0 ;
  }
}

public class MoveProcessY implements MoveProcess 
{
  public float getSpeed(float alpha) 
  {
    return (cos(4 * alpha)+1)*0.5;
    // return (icos((int)(RAD_TO_DEG * 4 * alpha))+1)*0.5;
  }
  public int getDirection(float alpha) {
    return (  (alpha >= 0 && alpha <= QUARTER_PI)||
      (alpha >= THREE_QUARTER_PI && alpha <= FIVE_QUARTER_PI)||
      (alpha >= SEVEN_QUARTER_PI && alpha <= TWO_PI)) ? 1 : 0 ;
  }
}

public interface MoveProcess 
{
  public float getSpeed(float alpha);
  public int getDirection(float alpha);
}

public class Drawer {
  color c;
  public PGraphics pg;
  Drawer(color c) {
    this.c = c;
    this.pg = createGraphics(width, height);
  }
  public void drawDuration(float x, float y) {
    pg.beginDraw();
    pg.strokeWeight(1);
    pg.stroke(c);
    pg.point(lerp(0, width, x), lerp(0, height, y));
    pg.endDraw();
  }
  public void drawDirection(float x, float y) {
    pg.beginDraw();
    pg.strokeWeight(4);
    pg.point(lerp(0, width, x), lerp((height>>1)-50, (height>>1)+50, y));
    pg.endDraw();
  }
  public void draw() {
    image(pg, 0, 0);
  }
}
