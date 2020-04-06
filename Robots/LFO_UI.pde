
class LFO_UI extends LFO {
  ArrayList <Float> points;
  boolean addEnable = true;
  float r = 1 ;
  public LFO_UI() {
    this(1.0f);
  }
  public LFO_UI(float frq) {
    this(frq, 1.0f);
  }
  public LFO_UI(float frq, float offset) {
    super(frq, offset);
    points = new ArrayList <Float>();
  } 

  public void update() {
    super.update();
    if(addEnable){
      points.add(this.getValue());
    }
    if (points.size()>200)points.remove(0);
  }

  public float draw(PApplet p) {
    float d = p.width/(float)points.size();
    float last = 0 ; 
    p.pushMatrix();
    p.translate(width - width * r, 0);
    for (int i = 1; i < points.size()*r; i ++) {
      float x0 = (i-1) * d;
      float y0 = points.get(i-1) * p.height;

      float x1 = (i) * d;
      float y1 = points.get(i) * p.height;
      last = points.get(i);
      p.line(x0, y0, x1, y1);
    }
    p.popMatrix();
    return constrain(last, 0, 1);
  }
}
