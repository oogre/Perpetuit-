

class LFO {
  private float frq;
  private float time;
  private float offset;
  public LFO() {
    this(1.0f);
  }
  public LFO(float frq) {
    this(frq, 1.0f);
  }
  public LFO(float frq, float offset) {
    this.frq = frq;
    this.offset = offset;
  } 
  private float frac(float v) {
    return v - (long)v;
  }
  public void update() {
    time += frq * TOOLS.getDeltaTime();

    time = frac(time);
  }
  public float getValue() {
    return (cos(time * TWO_PI)+1)*0.5;
  }
  public void setFrequency(float frq) {
    this.frq = frq;
  }
  public void reset() {
    time = 0.f;
  }
  public void setOffset(float offset) {
    this.offset = offset;
  }
};
