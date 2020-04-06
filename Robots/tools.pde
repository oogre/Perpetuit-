
static class TOOLS {
  protected static float time;
  protected static float deltaTime = 0.0f;
  protected static float lastFrameTime;
  public static void update(float t) {
    lastFrameTime = time;
    time = t * 0.001;
    deltaTime = time - lastFrameTime;
  }
  public static float getDeltaTime() {
    return deltaTime;
  }
};
