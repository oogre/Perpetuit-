import controlP5.*;
ControlP5 cp5;

Robot drawer, eraser;

public void settings() {
  size(1000, 200);
}

void setup() {
  cp5 = new ControlP5(this);

  drawer = new Robot(cp5, "drawer");
  eraser = new Robot(cp5, "eraser");
  surface.setLocation(0, 0);
  drawer.setLocation(0, height + 23);
  eraser.setLocation(0, height + drawer.height + 46);
}

void draw() {
  background(0);
  TOOLS.update(millis());
  eraser.setPoints(drawer.getPoints());
  int k = 20;
  for (int i = 0; i < k; i ++) {
    if (round(drawer.v1 * k) == i ) {
      print("·");
    } else if (round(eraser.v1 * k) == i ) {
      print("·");
    } else {
      print(" ");
    }
  }
  print(" | ");
  for (int i = 0; i < k; i ++) {
    if (round(drawer.v2 * k) == i ) {
      print("·");
    } else if (round(eraser.v2 * k) == i ) {
      print("·");
    } else {
      print(" ");
    }
  }

  println("");
}
