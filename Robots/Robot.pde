int i = 0 ;
class Robot extends PApplet {
  LFO_UI lfo1, lfo2;
  Slider s1;
  float v1, v2;
  Robot(ControlP5 p5, String name) {
    super();
    PApplet.runSketch(new String[] {this.getClass().getSimpleName()}, this);
    Group g1 = cp5.addGroup(name)
      .setPosition(400 * (i ++), 23)
      ;
    s1 = cp5.addSlider(name+" Offset")
      .setPosition(0, 0)
      .setSize(300, 20)
      .setRange(0, 1)
      .setValue(1)
      .setGroup(g1)
      ;
    s1.addCallback(new CallbackListener() {
      public void controlEvent(CallbackEvent theEvent) {
        if (theEvent.getAction()==ControlP5.ACTION_BROADCAST) {
          lfo1.r = s1.getValue();
          lfo2.r = s1.getValue();
        }
      }
    });
    
    if(name.equals("eraser")){
      s1.setValue(0.85);
    }
  }

  void settings() {
    size(1000, 200);
  }
  Robot setLocation(int x, int y) {
    surface.setLocation(x, y);
    return this;
  }

  void setup() {
    lfo1 = new LFO_UI(0.5);
    lfo2 = new LFO_UI(1);
  }

  void draw() {
    background(255);
    lfo1.update();
    lfo2.update();

    stroke(0, 0, 255);
    v1 = lfo1.draw(this);
    stroke(255, 0, 255);
    v2 = lfo2.draw(this);
  }
  
  ArrayList<Float> [] getPoints(){
    lfo1.addEnable = true;
    lfo2.addEnable = true;
    
    ArrayList<Float>[] al = new ArrayList[2]; 
    al[0] = lfo1.points;
    al[1] = lfo2.points;
    return al;
  }
  
  void setPoints(ArrayList<Float>[] al){
    lfo1.addEnable = false;
    lfo2.addEnable = false;
    lfo1.points = al[0];
    lfo2.points = al[1];
  }

  Robot reset() {
    lfo1.reset();
    lfo2.reset();
    return this;
  }
}
